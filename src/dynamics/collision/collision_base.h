////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_BASE_H
#define COLLISION_BASE_H

#include "collision_defs.h"

#include <qjsonarray.h>
#include <qvector3d.h>
#include <ode/ode.h>

class Collision
{
public:
    explicit Collision(dSpaceID space) : m_spaceID(space), m_geomID(0) {}

    virtual ~Collision() {
        if (m_geomID) {
            dGeomDestroy(m_geomID);
        }
    }

    inline dGeomID getGeomID() const { return m_geomID; }
    inline dSpaceID getSpaceID() const { return m_spaceID; }

    virtual void setPosition(dReal x, dReal y, dReal z) {
        dGeomSetPosition(m_geomID, x, y, z);
    }

    void setPosition(const QVector3D& pos) {
        setPosition(pos.x(), pos.y(), pos.z());
    }

    void setPosition(const QJsonArray& pos) {
        setPosition(pos[0].toDouble(), pos[1].toDouble(), pos[2].toDouble());
    }

    virtual void setRotation(const dMatrix3 R) {
        dGeomSetRotation(m_geomID, R);
    }

    virtual ECollisionType getCollisionType() const = 0;
    virtual void getCollisionParams(dReal* params) const = 0;

protected:
    dGeomID m_geomID;
    dSpaceID m_spaceID;
};

#endif // COLLISION_BASE_H
