////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_BASE_CPP
#define COLLISION_BASE_CPP

#include "collision_base.h"
#include <qjsonobject.h>
#include <qjsonarray.h>

void Collision::destroy()
{
    if (m_geomID) {
        dGeomDestroy(m_geomID);
    }
}

void Collision::load(const QJsonObject &config)
{
    QJsonArray pos = config["position"].toArray();
    setPosition(QVector3D(pos[0].toDouble(), pos[1].toDouble(), pos[2].toDouble()));
}

void Collision::setPosition(dReal x, dReal y, dReal z)
{
    dGeomSetPosition(m_geomID, x, y, z);
}

void Collision::setPosition(const QVector3D &pos)
{
    setPosition(pos.x(), pos.y(), pos.z());
}

void Collision::setRotation(const dMatrix3 R)
{
    dGeomSetRotation(m_geomID, R);
}

#endif // COLLISION_BASE_CPP
