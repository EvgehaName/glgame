////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_DEFS_CPP
#define COLLISION_DEFS_CPP
#include "collision_defs.h"

ECollisionType intToCollisionType(int type)
{
    switch (type) {
        case 1:
            return ECollisionType::CollisionTypeBox;
            break;
        case 2:
            return ECollisionType::CollisionTypeCapsule;
        default:
            return ECollisionType::CollisionTypeNone;
    }
}

int collisionTypeInt(ECollisionType type)
{
    return static_cast<int>(type);
}

#endif // COLLISION_DEFS_CPP
