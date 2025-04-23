#include "cube.h"

Cube::Cube()
{
	const VertexData vertices[4] = {
		{ {-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },  /* нижн€€ права€ вершина */
		{ {0.5f, 0.5f, 0.0f},   {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },  /* верн€€ права€ вершина */
		{ {0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },  /* верхн€€ лева€ вершина */
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },  /* нижн€€ лева€ вершина  */
	};

	const int indices[6] = {
		0, 3, 1, 
		3, 2, 1
	};

	vertexLayout.addAttribute(GL_FLOAT, 3, GL_FALSE, 8 * sizeof(float), 0); /* ѕозици€   */
	vertexLayout.addAttribute(GL_FLOAT, 3, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float)); /* Ќормаль   */
	vertexLayout.addAttribute(GL_FLOAT, 2, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float)); /* –азвертка */

	load(vertices, indices, sizeof(vertices), sizeof(indices));
}

void Cube::render()
{
	pVertexArray->bind();
	pShader->setUniformValue("uModel", modelMatrix);
	RenderGeometry::render();
}

void Cube::load(const void* vData, const void* iData, int vSize, int iSize)
{
	pVertexArray->bind();
	if (pVertexBuffer->bind())
	{
		pVertexBuffer->allocate(vData, vSize);

		if (pIndexBuffer->bind())
		{
			pIndexBuffer->allocate(iData, iSize);
		}
	}
}
