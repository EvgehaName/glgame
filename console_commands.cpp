#include "console_commands.h"
#include <QApplication>

void GameQuitCommand::Execute()
{
    QApplication::quit();
}
