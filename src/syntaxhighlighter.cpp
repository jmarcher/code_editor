#include "syntaxhighlighter.h"
#include <QColor>

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // Initialize formats with AYU Light colors
    keywordFormat.setForeground(QColor("#FA8D3E"));
    keywordFormat.setFontWeight(QFont::Bold);

    classFormat.setForeground(QColor("#55B4D4"));

    singleLineCommentFormat.setForeground(QColor("#ABB0B6"));

    quotationFormat.setForeground(QColor("#86B300"));

    functionFormat.setForeground(QColor("#F2AE49"));

    numberFormat.setForeground(QColor("#A37ACC"));

    htmlTagFormat.setForeground(QColor("#55B4D4"));
    htmlTagFormat.setFontWeight(QFont::Bold);

    htmlAttributeFormat.setForeground(QColor("#F2AE49"));
}

void SyntaxHighlighter::setLanguage(Language lang)
{
    highlightingRules.clear();
    switch (lang) {
        case PHP:
            setupPhpRules();
            break;
        case JavaScript:
            setupJavaScriptRules();
            break;
        case HTML:
            setupHtmlRules();
            break;
    }
    rehighlight();
}

void SyntaxHighlighter::setupPhpRules()
{
    HighlightingRule rule;

    // PHP keywords
    const QString keywordPatterns[] = {
        "\\barray\\b", "\\bas\\b", "\\bbreak\\b", "\\bcase\\b", "\\bcatch\\b",
        "\\bclass\\b", "\\bconst\\b", "\\bcontinue\\b", "\\bdeclare\\b",
        "\\bdefault\\b", "\\bdo\\b", "\\belse\\b", "\\belseif\\b", "\\bendif\\b",
        "\\bendswitch\\b", "\\bendwhile\\b", "\\bextends\\b", "\\bfinal\\b",
        "\\bfinally\\b", "\\bfor\\b", "\\bforeach\\b", "\\bfunction\\b",
        "\\bglobal\\b", "\\bif\\b", "\\bimplements\\b", "\\binclude\\b",
        "\\binclude_once\\b", "\\binstanceof\\b", "\\binsteadof\\b",
        "\\binterface\\b", "\\bnamespace\\b", "\\bnew\\b", "\\bor\\b",
        "\\bprivate\\b", "\\bprotected\\b", "\\bpublic\\b", "\\breturn\\b",
        "\\bstatic\\b", "\\bswitch\\b", "\\bthrow\\b", "\\btrait\\b",
        "\\btry\\b", "\\buse\\b", "\\bvar\\b", "\\bwhile\\b"
    };

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // PHP class names
    rule.pattern = QRegularExpression("\\b[A-Z][A-Za-z0-9_]*\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // PHP single-line comments
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // PHP strings
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // PHP functions
    rule.pattern = QRegularExpression("\\b[a-zA-Z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // PHP numbers
    rule.pattern = QRegularExpression("\\b\\d+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::setupJavaScriptRules()
{
    HighlightingRule rule;

    // JavaScript keywords
    const QString keywordPatterns[] = {
        "\\bbreak\\b", "\\bcase\\b", "\\bcatch\\b", "\\bclass\\b", "\\bconst\\b",
        "\\bcontinue\\b", "\\bdebugger\\b", "\\bdefault\\b", "\\bdelete\\b",
        "\\bdo\\b", "\\belse\\b", "\\bexport\\b", "\\bextends\\b", "\\bfalse\\b",
        "\\bfinally\\b", "\\bfor\\b", "\\bfunction\\b", "\\bif\\b", "\\bimport\\b",
        "\\bin\\b", "\\binstanceof\\b", "\\bnew\\b", "\\bnull\\b", "\\breturn\\b",
        "\\bsuper\\b", "\\bswitch\\b", "\\bthis\\b", "\\bthrow\\b", "\\btrue\\b",
        "\\btry\\b", "\\btypeof\\b", "\\bvar\\b", "\\bvoid\\b", "\\bwhile\\b",
        "\\bwith\\b", "\\byield\\b", "\\blet\\b", "\\bconst\\b", "\\bawait\\b",
        "\\basync\\b"
    };

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // JavaScript class names
    rule.pattern = QRegularExpression("\\b[A-Z][A-Za-z0-9_]*\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // JavaScript single-line comments
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // JavaScript strings
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression("`.*`");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // JavaScript functions
    rule.pattern = QRegularExpression("\\b[a-zA-Z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // JavaScript numbers
    rule.pattern = QRegularExpression("\\b\\d+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::setupHtmlRules()
{
    HighlightingRule rule;

    // HTML tags
    rule.pattern = QRegularExpression("<[!?]?\\w+(?=\\s|>|/>)[^>]*>|</\\w+>");
    rule.format = htmlTagFormat;
    highlightingRules.append(rule);

    // HTML attributes
    rule.pattern = QRegularExpression("\\s+\\w+(?=\\s*=)");
    rule.format = htmlAttributeFormat;
    highlightingRules.append(rule);

    // HTML strings
    rule.pattern = QRegularExpression("\"[^\"]*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression("'[^']*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // HTML comments
    rule.pattern = QRegularExpression("<!--[^>]*-->");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    const auto &rules = std::as_const(highlightingRules);
    for (const HighlightingRule &rule : rules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
} 