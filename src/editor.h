#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QPainter>
#include "syntaxhighlighter.h"
#include "codecompleter.h"

class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void setLanguage(const QString &filePath);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    SyntaxHighlighter *highlighter;
    CodeCompleter *completer;
    void setupEditor();
    QString getLanguageFromExtension(const QString &filePath);
    void handleIndentation(QKeyEvent *event);
    int getIndentationLevel(const QTextBlock &block);
    QString getIndentationString(int level);
    bool isOpeningBrace(const QString &text);
    bool isClosingBrace(const QString &text);
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(Editor *editor) : QWidget(editor), editor(editor) {}

    QSize sizeHint() const override
    {
        return QSize(editor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        editor->lineNumberAreaPaintEvent(event);
    }

private:
    Editor *editor;
};

#endif // EDITOR_H 