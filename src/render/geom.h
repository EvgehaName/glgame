#pragma once

#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglfunctions.h>
#include <qopengltexture.h>
#include <qopenglbuffer.h>

class VertexData
{
public:
	QVector3D position;
	QVector3D normal;
	QVector2D uv;

	VertexData(const QVector3D& pos, const QVector3D& norm, const QVector2D uv) :
		position(pos), normal(norm), uv(uv) { }
};

class IRenderGeometry {
public:
	QOpenGLBuffer* pVertexBuffer{ nullptr };
	QOpenGLBuffer* pIndexBuffer{ nullptr };

	virtual void render() = 0;
	virtual ~IRenderGeometry() = default;
};

class RenderGeometry : public IRenderGeometry {
public:
	RenderGeometry();

	virtual void render();
	virtual void load(const void* vData, const void* iData, int vSize, int iSize);

	QOpenGLShaderProgram* shader() const { return pShader;  }
	void setShader(QOpenGLShaderProgram* shader);
	virtual ~RenderGeometry();
protected:
	QOpenGLVertexArrayObject* pVertexArray{ nullptr };
	QOpenGLShaderProgram* pShader{ nullptr };
};
