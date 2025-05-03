#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum Language {
        PHP,
        JavaScript,
        HTML
    };

    explicit SyntaxHighlighter(QTextDocument *parent = nullptr);
    void setLanguage(Language lang);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    // AYU Light color scheme formats
    QTextCharFormat keywordFormat;      // #FA8D3E
    QTextCharFormat classFormat;        // #55B4D4
    QTextCharFormat singleLineCommentFormat; // #ABB0B6
    QTextCharFormat quotationFormat;    // #86B300
    QTextCharFormat functionFormat;     // #F2AE49
    QTextCharFormat numberFormat;       // #A37ACC
    QTextCharFormat htmlTagFormat;      // #55B4D4
    QTextCharFormat htmlAttributeFormat; // #F2AE49

    void setupPhpRules();
    void setupJavaScriptRules();
    void setupHtmlRules();
};

#endif // SYNTAXHIGHLIGHTER_H 