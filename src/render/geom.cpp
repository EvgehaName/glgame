#include "geom.h"

RenderGeometry::RenderGeometry()
{
	pVertexArray = new QOpenGLVertexArrayObject();
    if (!pVertexArray->create()) {
        qFatal("Failed to create vertex array");
    }
	pVertexArray->bind();

	pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	pVertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (!pVertexBuffer->create()) {
        qFatal("Failed to create vertex buffer");
    }
    pVertexBuffer->bind();

	pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	pIndexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (!pIndexBuffer->create()) {
        qFatal("Failed to create index buffer");
    }
    pIndexBuffer->bind();

	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();

	/* position */
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	f->glEnableVertexAttribArray(0);

	/* normal */
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	f->glEnableVertexAttribArray(1);

	/* uv */
	f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	f->glEnableVertexAttribArray(2);
}

void RenderGeometry::render()
{
    pVertexArray->bind();
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RenderGeometry::load(const void* vData, const void* iData, int vSize, int iSize)
{
	pVertexArray->bind();

    pVertexBuffer->bind();
	pVertexBuffer->allocate(vData, vSize);

    pIndexBuffer->bind();
	pIndexBuffer->allocate(iData, iSize);
}

void RenderGeometry::setShader(QOpenGLShaderProgram* shader)
{
	Q_ASSERT(shader); 
	if (pShader != shader) {
		pShader = shader;
	}
}

RenderGeometry::~RenderGeometry()
{
	if (pVertexBuffer) {
		pVertexBuffer->destroy();
		pVertexBuffer = nullptr;
	}

	if (pIndexBuffer) {
		pIndexBuffer->destroy();
		pIndexBuffer = nullptr;
	}

#ifdef QT_DEBUG
	qDebug() << Q_FUNC_INFO;
#endif // QT_DEBUG

}
