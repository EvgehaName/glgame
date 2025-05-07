////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H
#include "collision_base.h"

class CollisionBox final : public Collision
{
public:
    explicit CollisionBox(dSpaceID space);
    explicit CollisionBox(dSpaceID space, dReal l, dReal w, dReal h);

    inline dReal length() const { return m_length; }
    inline dReal width() const { return m_width; }
    inline dReal height() const { return m_height; }

    void setSize(const QVector3D& size);
    void load(const QJsonObject &config) override;

    ECollisionType getCollisionType() const override;
    void getCollisionParams(dReal *params) const override;

private:
    dReal m_length, m_width, m_height;
};

#endif // COLLISION_BOX_H
