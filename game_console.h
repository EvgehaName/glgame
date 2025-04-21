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

    GameConsole(QWidget *parent = nullptr);

    void registerCommand(const QString& name, IConsoleCommand* cmd);

    template<typename T>
    void registerCommand(const QString& name)
    {
        m_commands[name] = std::make_unique<T>();
    }

    void executeCommand(const QString& name);

    void log(const QString& message, LogLevel level = LogLevel::Info);
    void debug(const QString &message) { log(message, LogLevel::Debug); }
    void info(const QString &message) { log(message, LogLevel::Info); }
    void warning(const QString &message) { log(message, LogLevel::Warning); }
    void error(const QString &message) { log(message, LogLevel::Error); }

protected:
    void showEvent(QShowEvent *event) override;

private:
    QTextEdit * pOutputTextEdit = nullptr;
    QLineEdit * pInputLineEdit = nullptr;
    std::unordered_map<QString, std::unique_ptr<IConsoleCommand>> m_commands;

    Q_SLOT void executeCommandInternal();
};

#endif // GAME_CONSOLE_H
