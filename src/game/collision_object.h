////////////////////////////////////////////////////////////////////////////
//	Created 	: 27.04.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_OBJECT_H_INCLUDED
#define COLLISION_OBJECT_H_INCLUDED

class QJsonArray;
class QVector3D;
#include <ode/ode.h>

namespace collision {
	class collision_object
	{
	public:
		collision_object(dSpaceID space, dReal lx, dReal ly, dReal lz);
		collision_object(dSpaceID space, dReal radius, dReal length);
		collision_object(dSpaceID space, const QJsonArray& array);
		collision_object(dGeomID geom);
		~collision_object();

		void set_position(dReal x, dReal y, dReal z) const;
		void set_position(const QJsonArray& array) const;
		void set_position(const QVector3D& position) const;

		bool check_collide(collision_object* other) const;
		bool check_collide(collision_object* other, dContactGeom* contact) const;

	private:
		dGeomID m_geom;

		void construct_capsule(dSpaceID space, dReal radius, dReal length);
		void construct_box(dSpaceID space, dReal lx, dReal ly, dReal lz);
	};
} // namespace collision

#endif //COLLISION_OBJECT_H_INCLUDED