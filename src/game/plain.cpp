#include "plain.h"

#include <qjsonarray.h>

Plain::Plain()
{
	const VertexData vertices[4] = {
        { {-1.0f, 1.0f, 0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
        { {1.0f, 1.0f, 0.0f},   {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
        { {1.0f, -1.0f, 0.0f},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
        { {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
	};

	const int indices[6] = {
		0, 3, 1, 
		3, 2, 1
	};

    RenderGeometry::load(vertices, indices, sizeof(vertices), sizeof(indices));
}

Plain::Plain(const QJsonArray& pos, const QJsonArray& rot, const QJsonArray& scl) : Plain()
{
    m_position.setX(pos[0].toDouble());
    m_position.setY(pos[1].toDouble());
    m_position.setZ(pos[2].toDouble());

    m_rotation.setX(rot[0].toDouble());
    m_rotation.setY(rot[1].toDouble());
    m_rotation.setZ(rot[2].toDouble());

    m_scale.setX(scl[0].toDouble());
    m_scale.setY(scl[1].toDouble());
    m_scale.setZ(scl[2].toDouble());
}
