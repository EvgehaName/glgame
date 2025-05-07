////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "collision_box.h"

#include <qjsonobject.h>
#include <qjsonarray.h>

CollisionBox::CollisionBox(dSpaceID space)
    : Collision(space)
    , m_length(0)
    , m_width(0)
    , m_height(0)
{

}

CollisionBox::CollisionBox(dSpaceID space, dReal l, dReal w, dReal h)
    : Collision(space), m_length(l), m_width(w), m_height(h)
{
    m_geomID = dCreateBox(m_spaceID, m_length, m_width, m_height);
}

void CollisionBox::setSize(const QVector3D &size)
{
    dGeomBoxSetLengths(m_geomID, size.x(), size.y(), size.z());
}

void CollisionBox::load(const QJsonObject &config)
{
    if (m_geomID) {
        destroy();
    }

    m_geomID = dCreateBox(m_spaceID, m_length, m_width, m_height);
    Collision::load(config);

    QJsonArray size = config["size"].toArray();
    setSize(QVector3D(size[0].toDouble(), size[1].toDouble(), size[2].toDouble()));
}

ECollisionType CollisionBox::getCollisionType() const
{
    return ECollisionType::CollisionTypeBox;
}

void CollisionBox::getCollisionParams(dReal *params) const
{
    params[0] = m_length;
    params[1] = m_width;
    params[2] = m_height;
}
