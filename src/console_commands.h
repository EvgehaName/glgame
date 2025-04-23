#ifndef CONSOLE_COMMANDS_H
#define CONSOLE_COMMANDS_H

#include <QString>
#include <QStringList>

class CCommand
{
public:
    CCommand(const QString& argv);

    int argc() const;
    QString argv(int index) const;

protected:
    QStringList m_argv;
};

#endif // CONSOLE_COMMANDS_H
