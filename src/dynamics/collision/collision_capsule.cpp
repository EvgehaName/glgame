////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "collision_capsule.h"

CollisionCapsule::CollisionCapsule(dSpaceID space)
    : Collision(space)
    , m_radius(0)
    , m_length(0)
{

}

CollisionCapsule::CollisionCapsule(dSpaceID space, dReal r, dReal l)
    : Collision(space), m_radius(r), m_length(l)
{
    m_geomID = dCreateCapsule(m_spaceID, m_radius, m_length);
}

ECollisionType CollisionCapsule::getCollisionType() const
{
    return ECollisionType::CollisionTypeCapsule;
}

void CollisionCapsule::getCollisionParams(dReal *params) const
{
    params[0] = m_radius;
    params[1] = m_length;
}
