////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_DEFS_H
#define COLLISION_DEFS_H

enum class ECollisionType {
    CollisionTypeNone,
    CollisionTypeBox,
    CollisionTypeCapsule
};

constexpr int collisionTypeInt(ECollisionType type) {
    return static_cast<int>(type);
}

#endif // COLLISION_DEFS_H
