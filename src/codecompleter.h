#ifndef CODECOMPLETER_H
#define CODECOMPLETER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QTextCursor>
#include <QCompleter>
#include <QAbstractItemView>

class Editor;

class CodeCompleter : public QObject
{
    Q_OBJECT

public:
    explicit CodeCompleter(Editor *editor);
    void setupCompleter();
    void updateCompletions(const QString &text, int cursorPosition);
    void showCompletions(const QPoint &pos);
    void hideCompletions();

private:
    Editor *editor;
    QCompleter *completer;
    QAbstractItemView *popup;
    QStringList wordList;
    QMap<QString, QString> laravelKeywords;
    QMap<QString, QString> laravelMethods;
    QMap<QString, QString> laravelClasses;
    
    void initializeLaravelKeywords();
    void initializeLaravelMethods();
    void initializeLaravelClasses();
    QString getCurrentWord(const QString &text, int cursorPosition);
    QStringList getCompletions(const QString &word);
    bool isInLaravelContext(const QString &text, int cursorPosition);
};

#endif // CODECOMPLETER_H 