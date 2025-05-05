////////////////////////////////////////////////////////////////////////////
//	Created 	: 04.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "rigidbody.h"

#include "collision/collision_box.h"
#include "collision/collision_capsule.h"

Rigidbody::Rigidbody(dWorldID world)
    : m_density(1.0f)
    , m_bodyID(nullptr)
    , m_worldID(world)
    , m_collision(nullptr)
{
    m_bodyID = dBodyCreate(m_worldID);
}

Rigidbody::~Rigidbody()
{
    if (m_bodyID) {
        dBodyDestroy(m_bodyID);
        m_bodyID = nullptr;
    }
}

void Rigidbody::setDensity(float density)
{
    m_density = density;
}

void Rigidbody::setPosition(const QVector3D &pos)
{
    setPosition(pos.x(), pos.y(), pos.z());
}

void Rigidbody::setPosition(float x, float y, float z)
{
    dBodySetPosition(m_bodyID, x, y, z);
}

QVector3D Rigidbody::position() const
{
    const auto pos = dBodyGetPosition(m_bodyID);
    return QVector3D(pos[0], pos[1], pos[2]);
}

void Rigidbody::setLinearDamping(float damping)
{
    dBodySetLinearDamping(m_bodyID, damping);
}

void Rigidbody::setAngularDamping(float damping)
{
    dBodySetAngularDamping(m_bodyID, damping);
}

void Rigidbody::setCollision(Collision *collision)
{
    dGeomSetBody(collision->getGeomID(), m_bodyID);
    m_collision = collision;

    dMass m;
    dMassSetZero(&m);

    switch (collision->getCollisionType()) {
        case ECollisionType::CollisionTypeBox:
        {
            CollisionBox * box = static_cast<CollisionBox*>(collision);
            dMassSetBox(&m, m_density, box->length(), box->width(), box->height());
            break;
        }
        case ECollisionType::CollisionTypeCapsule:
        {
            CollisionCapsule * capsule = static_cast<CollisionCapsule*>(collision);
            dMassSetCapsule(&m, m_density, 3, capsule->radius(), capsule->length());
            break;
        }
        default:
            break;
    }

    dBodySetMass(m_bodyID, &m);
}
