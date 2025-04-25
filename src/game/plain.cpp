#include "plain.h"

Plain::Plain()
{
	const VertexData vertices[4] = {
		{ {-1.0f, 1.0f, 0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },  /* нижн€€ права€ вершина */
		{ {1.0f, 1.0f, 0.0f},   {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },  /* верн€€ права€ вершина */
		{ {1.0f, -1.0f, 0.0f},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },  /* верхн€€ лева€ вершина */
		{ {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },  /* нижн€€ лева€ вершина  */
	};

	const int indices[6] = {
		0, 3, 1, 
		3, 2, 1
	};

	load(vertices, indices, sizeof(vertices), sizeof(indices));
}

void Plain::render()
{
	const QMatrix4x4& model = getModelMatrix();
	pShader->setUniformValue("uModel", model);
	RenderGeometry::render();
}
