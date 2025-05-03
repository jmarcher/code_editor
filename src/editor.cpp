#include "editor.h"
#include <QPainter>
#include <QTextBlock>
#include <QFont>
#include <QFontMetrics>
#include <QFileInfo>
#include <QKeyEvent>
#include <QTextCursor>

Editor::Editor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    highlighter = new SyntaxHighlighter(document());
    completer = new CodeCompleter(this);
    setupEditor();

    connect(this, &Editor::blockCountChanged, this, &Editor::updateLineNumberAreaWidth);
    connect(this, &Editor::updateRequest, this, &Editor::updateLineNumberArea);

    updateLineNumberAreaWidth(0);
}

void Editor::setupEditor()
{
    // Set AYU Light color scheme
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor("#FAFAFA"));
    p.setColor(QPalette::Text, QColor("#5C6773"));
    p.setColor(QPalette::Highlight, QColor("#D4D4D4"));
    p.setColor(QPalette::HighlightedText, QColor("#5C6773"));
    setPalette(p);

    // Set font
    QFont font("Monaco", 12);
    font.setFixedPitch(true);
    setFont(font);

    // Enable line wrapping
    setLineWrapMode(QPlainTextEdit::NoWrap);

    // Set tab width to 4 spaces
    setTabStopDistance(fontMetrics().horizontalAdvance(' ') * 4);
}

QString Editor::getLanguageFromExtension(const QString &filePath)
{
    QString extension = QFileInfo(filePath).suffix().toLower();
    
    if (extension == "php") {
        return "php";
    } else if (extension == "js") {
        return "javascript";
    } else if (extension == "html" || extension == "htm") {
        return "html";
    }
    
    return QString();
}

void Editor::setLanguage(const QString &filePath)
{
    QString language = getLanguageFromExtension(filePath);
    
    if (language == "php") {
        highlighter->setLanguage(SyntaxHighlighter::PHP);
    } else if (language == "javascript") {
        highlighter->setLanguage(SyntaxHighlighter::JavaScript);
    } else if (language == "html") {
        highlighter->setLanguage(SyntaxHighlighter::HTML);
    }
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        handleIndentation(event);
    } else {
        QPlainTextEdit::keyPressEvent(event);
        
        // Update completions after key press
        QTextCursor cursor = textCursor();
        QString text = toPlainText();
        completer->updateCompletions(text, cursor.position());
    }
}

void Editor::handleIndentation(QKeyEvent *event)
{
    QTextCursor cursor = textCursor();
    QTextBlock currentBlock = cursor.block();
    QString currentLine = currentBlock.text();
    int currentIndent = getIndentationLevel(currentBlock);
    int cursorPosition = cursor.positionInBlock();

    // Check if we're at the end of a line with an opening brace
    bool hasOpeningBrace = false;
    if (cursorPosition == currentLine.length()) {
        QString textBeforeCursor = currentLine.left(cursorPosition);
        hasOpeningBrace = isOpeningBrace(textBeforeCursor);
    }

    // Insert newline and maintain indentation
    QPlainTextEdit::keyPressEvent(event);

    // Get the new cursor position after the newline
    cursor = textCursor();

    // Set the indentation for the new line
    QString indent = getIndentationString(currentIndent);
    if (hasOpeningBrace) {
        // If there was an opening brace, increase indentation
        indent += getIndentationString(1);
    }

    cursor.insertText(indent);

    // If we had an opening brace, also add a closing brace on the previous line
    if (hasOpeningBrace) {
        cursor.movePosition(QTextCursor::PreviousBlock);
        cursor.movePosition(QTextCursor::EndOfBlock);
        cursor.insertText("\n" + getIndentationString(currentIndent) + "}");
        cursor.movePosition(QTextCursor::PreviousBlock);
        cursor.movePosition(QTextCursor::EndOfBlock);
        setTextCursor(cursor);
    }
}

int Editor::getIndentationLevel(const QTextBlock &block)
{
    QString text = block.text();
    int level = 0;
    while (level < text.length() && (text[level] == ' ' || text[level] == '\t')) {
        level++;
    }
    return level / 4; // Assuming 4 spaces per indentation level
}

QString Editor::getIndentationString(int level)
{
    return QString(level * 4, ' '); // 4 spaces per indentation level
}

bool Editor::isOpeningBrace(const QString &text)
{
    return text.contains('{') && !text.contains('}');
}

bool Editor::isClosingBrace(const QString &text)
{
    return text.contains('}') && !text.contains('{');
}

int Editor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void Editor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor("#FAFAFA"));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor("#5C6773"));
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                           Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
} 