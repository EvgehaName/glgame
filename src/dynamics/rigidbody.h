////////////////////////////////////////////////////////////////////////////
//	Created 	: 04.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "collision/collision_base.h"
#include <QVector3D>

class Rigidbody final
{
public:
    Rigidbody(dWorldID world);
    ~Rigidbody();

    void setDensity(float density);
    void setPosition(const QVector3D& pos);
    void setPosition(float x, float y, float z);
    QVector3D position() const;

    void setLinearDamping(float damping);
    void setAngularDamping(float damping);
    inline dBodyID getBodyID() const { return m_bodyID; }

    void setCollision(Collision * collision);
    inline const Collision * collision() const { return m_collision; }

    Rigidbody(const Rigidbody&) = delete; // non construction-copyable
    Rigidbody& operator=(const Rigidbody&) = delete; // non copyable

private:
    float m_density;
    dBodyID m_bodyID;
    dWorldID m_worldID;
    Collision * m_collision;
};

#endif // RIGIDBODY_H
