////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "collision_factory.h"
#include "collision_capsule.h"
#include "../physics_world.h"
#include "collision_base.h"
#include "collision_box.h"

Collision *CollisionFactory::createCollision(ECollisionType type)
{
    auto& phWorld = PhysicsWorld::getInstance();

    switch (type) {
        case ECollisionType::CollisionTypeBox:
            return new CollisionBox(phWorld.getSpace());
        case ECollisionType::CollisionTypeCapsule:
            return new CollisionCapsule(phWorld.getSpace());
        default:
            return nullptr;
    }
}
