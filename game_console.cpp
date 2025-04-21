#include "game_console.h"
#include "console_commands.h"

GameConsole::GameConsole(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::BlankCursor);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    pOutputTextEdit = new QTextEdit(this);
    pOutputTextEdit->setReadOnly(true);
    pOutputTextEdit->setCursor(Qt::BlankCursor);
    pOutputTextEdit->setFrameShape(QFrame::NoFrame);
    pOutputTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    pOutputTextEdit->setStyleSheet(
        "QTextEdit {"
        "   background: rgba(30, 30, 30, 90);"
        "   font-family: Consolas, monospace;"
        "   font-size: 10pt;"
        "}"
    );

    pInputLineEdit = new QLineEdit(this);
    pInputLineEdit->setFrame(false);
    pInputLineEdit->setStyleSheet(
        "QLineEdit {"
        "   background: rgba(30, 30, 30, 150);"
        "   color: white;"
        "   border: 1px solid rgba(100, 100, 100, 150);"
        "   padding: 2px;"
        "   font-family: Consolas, monospace;"
        "   font-size: 10pt;"
        "}"
        );
    pInputLineEdit->setCursor(Qt::BlankCursor);

    layout->addWidget(pOutputTextEdit);
    layout->addWidget(pInputLineEdit);

    connect(pInputLineEdit, &QLineEdit::returnPressed, this, &GameConsole::executeCommandInternal);
}

void GameConsole::registerCommand(const QString &name, IConsoleCommand *cmd)
{
    m_commands[name] = std::unique_ptr<IConsoleCommand>(cmd);
}

void GameConsole::executeCommand(const QString &name)
{
    auto it = m_commands.find(name);
    if (it != m_commands.end()) {
        it->second->Execute();
    } else {
        warning(QString("Unknown command: %1").arg(name));
    }

    pInputLineEdit->clear();
}

void GameConsole::log(const QString &message, LogLevel level)
{
    QString timestamp   = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString levelString = "<info>";
    QColor levelColor   = QColor("#ffffff");

    switch(level)
    {
    case Info:
        levelString = "info";
        break;
    case Debug:
        levelString = "debug";
        break;
    case Error:
        levelString = "ERROR";
        levelColor = QColor("#ff0000");
        break;
    case Warning:
        levelString = "Warning";
        levelColor = QColor("#ffff00");
        break;
    }

    QString logEntry = QString("<%1>[%2] %3").arg(levelString, timestamp, message);

    QColor defColor = pOutputTextEdit->textColor();
    pOutputTextEdit->setTextColor(levelColor);
    pOutputTextEdit->append(logEntry);
    pOutputTextEdit->setTextColor(defColor);

    QScrollBar * scrollBar = pOutputTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void GameConsole::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    pInputLineEdit->setFocus();
}

void GameConsole::executeCommandInternal()
{
    QString input = pInputLineEdit->text().trimmed();
    executeCommand(input);
}
