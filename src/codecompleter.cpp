#include "codecompleter.h"
#include "editor.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QApplication>
#include <QScreen>

CodeCompleter::CodeCompleter(Editor *editor) : QObject(editor), editor(editor)
{
    setupCompleter();
    initializeLaravelKeywords();
    initializeLaravelMethods();
    initializeLaravelClasses();
}

void CodeCompleter::setupCompleter()
{
    completer = new QCompleter(this);
    completer->setWidget(editor);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    
    popup = completer->popup();
    popup->setStyleSheet("QListView { background-color: #FAFAFA; border: 1px solid #D4D4D4; }"
                        "QListView::item { padding: 2px; }"
                        "QListView::item:selected { background-color: #D4D4D4; color: #5C6773; }");
}

void CodeCompleter::initializeLaravelKeywords()
{
    laravelKeywords = {
        {"Route", "Route::get('/path', [Controller::class, 'method']);"},
        {"View", "View::make('view.name', ['key' => 'value']);"},
        {"Controller", "class Controller extends BaseController {}"},
        {"Model", "class Model extends Model {}"},
        {"DB", "DB::table('table')->get();"},
        {"Auth", "Auth::user();"},
        {"Session", "Session::put('key', 'value');"},
        {"Cache", "Cache::put('key', 'value', $minutes);"},
        {"Config", "Config::get('key');"},
        {"Request", "Request::input('key');"},
        {"Response", "Response::json(['key' => 'value']);"},
        {"Validator", "Validator::make($data, $rules);"},
        {"Redirect", "Redirect::to('url');"},
        {"URL", "URL::to('path');"},
        {"Asset", "Asset::url('path');"},
        {"Storage", "Storage::put('file.txt', $contents);"},
        {"Log", "Log::info('message');"},
        {"Event", "Event::dispatch(new EventClass());"},
        {"Queue", "Queue::push(new JobClass());"},
        {"Mail", "Mail::to($user)->send(new Mailable());"}
    };
}

void CodeCompleter::initializeLaravelMethods()
{
    laravelMethods = {
        {"get", "get() - Get all records"},
        {"first", "first() - Get first record"},
        {"find", "find($id) - Find record by ID"},
        {"where", "where('column', 'operator', 'value') - Add where clause"},
        {"orderBy", "orderBy('column', 'direction') - Order results"},
        {"limit", "limit($count) - Limit number of results"},
        {"offset", "offset($count) - Skip number of results"},
        {"count", "count() - Count records"},
        {"sum", "sum('column') - Sum column values"},
        {"avg", "avg('column') - Average column values"},
        {"max", "max('column') - Maximum column value"},
        {"min", "min('column') - Minimum column value"},
        {"pluck", "pluck('column') - Get single column values"},
        {"select", "select('column1', 'column2') - Select specific columns"},
        {"join", "join('table', 'column1', '=', 'column2') - Join tables"},
        {"leftJoin", "leftJoin('table', 'column1', '=', 'column2') - Left join tables"},
        {"groupBy", "groupBy('column') - Group results"},
        {"having", "having('column', 'operator', 'value') - Add having clause"},
        {"distinct", "distinct() - Get distinct results"},
        {"exists", "exists() - Check if record exists"}
    };
}

void CodeCompleter::initializeLaravelClasses()
{
    laravelClasses = {
        {"App\\Models\\", "Base model class"},
        {"App\\Http\\Controllers\\", "Controller classes"},
        {"App\\Http\\Requests\\", "Form request classes"},
        {"App\\Http\\Middleware\\", "Middleware classes"},
        {"App\\Providers\\", "Service provider classes"},
        {"App\\Services\\", "Service classes"},
        {"App\\Events\\", "Event classes"},
        {"App\\Listeners\\", "Event listener classes"},
        {"App\\Jobs\\", "Job classes"},
        {"App\\Mail\\", "Mail classes"},
        {"App\\Notifications\\", "Notification classes"},
        {"App\\Policies\\", "Policy classes"},
        {"App\\Rules\\", "Validation rule classes"},
        {"App\\Exceptions\\", "Exception classes"},
        {"App\\Console\\Commands\\", "Artisan command classes"}
    };
}

QString CodeCompleter::getCurrentWord(const QString &text, int cursorPosition)
{
    int start = cursorPosition;
    while (start > 0 && (text[start - 1].isLetterOrNumber() || text[start - 1] == '_' || text[start - 1] == '\\')) {
        --start;
    }
    return text.mid(start, cursorPosition - start);
}

QStringList CodeCompleter::getCompletions(const QString &word)
{
    QStringList completions;
    
    // Check for Laravel keywords
    for (const QString &key : laravelKeywords.keys()) {
        if (key.startsWith(word, Qt::CaseInsensitive)) {
            completions << key;
        }
    }
    
    // Check for Laravel methods
    for (const QString &key : laravelMethods.keys()) {
        if (key.startsWith(word, Qt::CaseInsensitive)) {
            completions << key;
        }
    }
    
    // Check for Laravel classes
    for (const QString &key : laravelClasses.keys()) {
        if (key.startsWith(word, Qt::CaseInsensitive)) {
            completions << key;
        }
    }
    
    return completions;
}

bool CodeCompleter::isInLaravelContext(const QString &text, int cursorPosition)
{
    // Check if we're in a PHP file
    if (!text.contains("<?php")) {
        return false;
    }
    
    // Check if we're in a Laravel-specific context
    QString beforeCursor = text.left(cursorPosition);
    return beforeCursor.contains("namespace") || 
           beforeCursor.contains("use") || 
           beforeCursor.contains("class") ||
           beforeCursor.contains("function");
}

void CodeCompleter::updateCompletions(const QString &text, int cursorPosition)
{
    if (!isInLaravelContext(text, cursorPosition)) {
        hideCompletions();
        return;
    }
    
    QString currentWord = getCurrentWord(text, cursorPosition);
    if (currentWord.length() < 2) {
        hideCompletions();
        return;
    }
    
    QStringList completions = getCompletions(currentWord);
    if (completions.isEmpty()) {
        hideCompletions();
        return;
    }
    
    completer->setModel(new QStringListModel(completions, completer));
    completer->setCompletionPrefix(currentWord);
    
    QTextCursor cursor = editor->textCursor();
    QRect rect = editor->cursorRect(cursor);
    showCompletions(rect.bottomLeft());
}

void CodeCompleter::showCompletions(const QPoint &pos)
{
    if (!completer->completionCount()) {
        hideCompletions();
        return;
    }
    
    QScreen *screen = QApplication::screenAt(editor->mapToGlobal(pos));
    if (!screen) {
        screen = QApplication::primaryScreen();
    }
    
    QRect screenGeometry = screen->availableGeometry();
    QPoint popupPos = editor->mapToGlobal(pos);
    
    if (popupPos.x() + popup->width() > screenGeometry.right()) {
        popupPos.setX(screenGeometry.right() - popup->width());
    }
    if (popupPos.y() + popup->height() > screenGeometry.bottom()) {
        popupPos.setY(pos.y() - popup->height() - editor->fontMetrics().height());
    }
    
    popup->move(popupPos);
    popup->show();
}

void CodeCompleter::hideCompletions()
{
    if (popup) {
        popup->hide();
    }
} 