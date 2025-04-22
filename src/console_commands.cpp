#include "console_commands.h"

CCommand::CCommand(const QString &argv)
{
    m_argv = argv.split(' ', Qt::SkipEmptyParts);
}

int CCommand::argc() const
{
    return m_argv.size();
}

QString CCommand::argv(int index) const
{
    if (index > -1 && index < argc()) {
        return m_argv[index];
    }

    return QString("");
}
