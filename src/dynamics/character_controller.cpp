////////////////////////////////////////////////////////////////////////////
//	Created 	: 04.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
///////////////////////////////////////////////////////////////////////////
#include "character_controller.h"

CharacterController::CharacterController(dWorldID world, dSpaceID space)
    : m_mass(70.0f)
    , m_bodyID(nullptr)
    , m_rigidbody(nullptr)
    , m_capsule(nullptr)
{
    m_capsule = new CollisionCapsule(space, 0.2f, 1.0f);

    m_rigidbody = new Rigidbody(world);
    m_rigidbody->setCollision(m_capsule);
    m_bodyID = m_rigidbody->getBodyID();

    // dMass m;
    // dMassSetSphere(&m, 1, 1000000.f / 2.f);
    // dMassAdjust(&m, m_mass);
    // dBodySetMass(m_bodyID, &m);

    dMatrix3 R;
    dRFromAxisAndAngle(R, 1, 0, 0, M_PI / 2);
    dBodySetRotation(m_rigidbody->getBodyID(), R);

    //fixRotation();
}

CharacterController::~CharacterController()
{
    if (m_rigidbody) {
        delete m_rigidbody;
    }

    if (m_capsule) {
        delete m_capsule;
    }
}

void CharacterController::setPosition(const QVector3D &position)
{
    m_rigidbody->setPosition(position);
    fixRotation();
}

void CharacterController::fixRotation()
{
    dMatrix3 R;
    dRSetIdentity(R);
    dRFromAxisAndAngle(R, 1, 0, 0, M_PI / 2);
    dBodySetAngularVel(m_bodyID, 0.f, 0.f, 0.f);
    dBodySetRotation(m_bodyID, R);
}

