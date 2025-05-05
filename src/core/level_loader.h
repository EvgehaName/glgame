////////////////////////////////////////////////////////////////////////////
//	Created 	: 27.04.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef LEVEL_PARSER_H_INCLUDED
#define LEVEL_PARSER_H_INCLUDED

#include <qstring.h>

class Level;

class LevelLoader
{
public:
    LevelLoader() = default;

    bool load(const QString& filepath, Level* out);
    inline const QString& getLog() const { return m_log; }
private:
    QString m_log;
};

#endif //LEVEL_PARSER_H_INCLUDED
