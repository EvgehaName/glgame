#ifndef CONSOLE_COMMANDS_H
#define CONSOLE_COMMANDS_H

class IConsoleCommand {
public:
    virtual void Execute() = 0;
    virtual ~IConsoleCommand() = default;
};

class GameQuitCommand : public IConsoleCommand {
public:
    void Execute();
};

#endif // CONSOLE_COMMANDS_H
