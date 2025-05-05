////////////////////////////////////////////////////////////////////////////
//	Created 	: 04.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include "collision/collision_capsule.h"
#include "rigidbody.h"

class CharacterController final
{
public:
    CharacterController(dWorldID world, dSpaceID space);
    ~CharacterController();

    void setPosition(const QVector3D& position);
    inline QVector3D position() const { return m_rigidbody->position(); }

    inline Rigidbody* rigidbody() const { return m_rigidbody; }
    inline CollisionCapsule* collsiion() const { return m_capsule; }

private:
    float m_mass;
    dBodyID m_bodyID;
    Rigidbody * m_rigidbody;
    CollisionCapsule * m_capsule;

    void fixRotation();
};

#endif // CHARACTER_CONTROLLER_H
