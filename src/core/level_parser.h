////////////////////////////////////////////////////////////////////////////
//	Created 	: 27.04.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef LEVEL_PARSER_H_INCLUDED
#define LEVEL_PARSER_H_INCLUDED

#include <qstring.h>

class Level;

class LevelParser
{
public:
	static bool parse(const QString& filepath, Level* out);
};

#endif //LEVEL_PARSER_H_INCLUDED