#ifndef GAME_CONSOLE_H
#define GAME_CONSOLE_H

#include <QEvent>
#include <QShowEvent>

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QDateTime>
#include <QScrollBar>
#include <QVBoxLayout>

#include "console_commands.h"

class IConsoleCommand;

class GameConsole : public QWidget
{
    Q_OBJECT
public:
    enum LogLevel {
        Info,
        Debug,
        Error,
        Warning
    };

    using CommandCallback = std::function<void(const CCommand&)>;

    GameConsole(QWidget *parent = nullptr);

    bool consoleHasFocus() const;
    void registerCommand(const QString& name, CommandCallback command);
    void executeCommand(const QString& argv);

    void log(const QString& message, LogLevel level = LogLevel::Info);
    void debug(const QString &message) { log(message, LogLevel::Debug); }
    void info(const QString &message) { log(message, LogLevel::Info); }
    void warning(const QString &message) { log(message, LogLevel::Warning); }
    void error(const QString &message) { log(message, LogLevel::Error); }

protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;

private:
    QTextEdit * pOutputTextEdit = nullptr;
    QLineEdit * pInputLineEdit = nullptr;

    std::unordered_map<QString, CommandCallback> m_commands;
    Q_SLOT void executeCommandInternal();
};

#endif // GAME_CONSOLE_H
