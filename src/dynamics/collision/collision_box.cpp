////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "collision_box.h"

CollisionBox::CollisionBox(dSpaceID space, dReal l, dReal w, dReal h)
    : Collision(space), m_length(l), m_width(w), m_height(h)
{
    m_geomID = dCreateBox(m_spaceID, m_length, m_width, m_height);
}

CollisionBox::CollisionBox(dSpaceID space, const QJsonArray &size)
    : CollisionBox(space, size[0].toDouble(), size[1].toDouble(), size[2].toDouble())
{

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
