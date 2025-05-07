////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_BASE_H
#define COLLISION_BASE_H

#include "collision_defs.h"
#include <qvector3d.h>
#include <ode/ode.h>

/* Forward declaration */
class QJsonObject;

class Collision
{
public:
    explicit Collision(dSpaceID space) : m_spaceID(space), m_geomID(nullptr) {}
    virtual ~Collision() {
        destroy();
    }

    void destroy();
    virtual void load(const QJsonObject& config);

    inline dGeomID getGeomID() const { return m_geomID; }
    inline dSpaceID getSpaceID() const { return m_spaceID; }

    virtual void setPosition(dReal x, dReal y, dReal z);
    void setPosition(const QVector3D& pos);
    virtual void setRotation(const dMatrix3 R);

    virtual ECollisionType getCollisionType() const = 0;
    virtual void getCollisionParams(dReal* params) const = 0;

protected:
    dGeomID m_geomID;
    dSpaceID m_spaceID;
};

#endif // COLLISION_BASE_H
