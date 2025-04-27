////////////////////////////////////////////////////////////////////////////
//	Created 	: 27.04.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "collision_object.h"
#include <qjsonarray.h>
#include <qvector3d.h>

namespace collision {
	collision_object::collision_object(dSpaceID space, dReal lx, dReal ly, dReal lz)
	{
		construct_box(space, lx, ly, lz);
	}

	collision_object::collision_object(dSpaceID space, dReal radius, dReal length)
	{
		construct_capsule(space, radius, length);
	}

	collision_object::collision_object(dSpaceID space, const QJsonArray& array)
	{
		construct_box(space, array[0].toDouble(), array[1].toDouble(), array[2].toDouble());
	}

	collision_object::collision_object(dGeomID geom)
	{
		m_geom = geom;
	}

	collision_object::~collision_object()
	{
		if (m_geom) {
			dGeomDestroy(m_geom);
		}
	}

	void collision_object::set_position(dReal x, dReal y, dReal z) const
	{
		dGeomSetPosition(m_geom, x, y, z);
	}

	void collision_object::set_position(const QJsonArray& array) const
	{
		set_position(array[0].toDouble(), array[1].toDouble(), array[2].toDouble());
	}

	void collision_object::set_position(const QVector3D& position) const
	{
		set_position(position.x(), position.y(), position.z());
	}

	bool collision_object::check_collide(collision_object* other) const
	{
		dContactGeom contact{};
		int contacts_number = dCollide(m_geom, other->m_geom, 1, &contact, sizeof(dContactGeom));
		if (contacts_number != 0)
		{
			qDebug() << contacts_number;
		}
		if (contacts_number > 0) {
			return true;
		}

		return false;
	}

	bool collision_object::check_collide(collision_object* other, dContactGeom* contact) const
	{
		int contacts_number = dCollide(m_geom, other->m_geom, 1, contact, sizeof(dContactGeom));
		if (contacts_number > 0) {
			return true;
		}

		return false;
	}

	void collision_object::construct_capsule(dSpaceID space, dReal radius, dReal length)
	{
		m_geom = dCreateCapsule(space, radius, length);
	}

	void collision_object::construct_box(dSpaceID space, dReal lx, dReal ly, dReal lz)
	{
		m_geom = dCreateBox(space, lx, ly, lz);
	}
} // namespace collision