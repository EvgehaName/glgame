////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_FACTORY_H
#define COLLISION_FACTORY_H

/* Forward declaration */
class Collision;
enum class ECollisionType;

class CollisionFactory final
{
public:
    static Collision* createCollision(ECollisionType type);

    CollisionFactory(const CollisionFactory&) = delete; // non construction-copyable
    CollisionFactory& operator=(const CollisionFactory&) = delete; // non copyable

private:
    CollisionFactory() = default;
};

#endif // COLLISION_FACTORY_H
