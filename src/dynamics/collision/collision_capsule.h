////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_CAPSULE_H
#define COLLISION_CAPSULE_H
#include "collision_base.h"

class CollisionCapsule final : public Collision
{
public:
    explicit CollisionCapsule(dSpaceID space);
    explicit CollisionCapsule(dSpaceID space, dReal r, dReal l);

    inline dReal radius() const { return m_radius; }
    inline dReal length() const { return m_length; }

    ECollisionType getCollisionType() const override;
    void getCollisionParams(dReal *params) const override;

private:
    dReal m_radius;
    dReal m_length;
};

#endif // COLLISION_CAPSULE_H
