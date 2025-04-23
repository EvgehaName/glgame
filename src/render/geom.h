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

class VertexLayout
{
public:
	struct VertexAttribute
	{
		GLenum type;           // Тип данных (GL_FLOAT, GL_INT и т.д.)
		GLint size;            // Количество компонентов (1, 2, 3, 4)
		GLboolean normalized;  // Нормализовать ли данные
		GLsizei stride;        // Шаг между атрибутами
		GLsizei offset;        // Смещение в байтах
	};

	void addAttribute(GLenum type, GLint size, GLboolean normalized = GL_FALSE, GLsizei stride = 0, GLsizei offset = 0);
	void addAttribute(const VertexAttribute& attr);
	void enableAttributes(QOpenGLShaderProgram* program);

	inline bool isEnabledAttr() const { return m_enabledAttr;}
	inline GLsizei getStride() const { return m_stride; }

private:
	QVector<VertexAttribute> m_attributes;
	bool m_enabledAttr{ false };
	GLsizei m_stride{ 0 };
};

class IRenderGeometry {
public:
	QOpenGLBuffer* pVertexBuffer{ nullptr };
	QOpenGLBuffer* pIndexBuffer{ nullptr };
	virtual ~IRenderGeometry() = default;
};

class RenderGeometry : public IRenderGeometry {
public:
	RenderGeometry();

	virtual void render();
	virtual void load(const void* vData, const void* iData, int vSize, int iSize) = 0;

	void insertTexture(QOpenGLTexture* texture, int slot);
	void setShader(QOpenGLShaderProgram* shader);
	virtual ~RenderGeometry();
protected:
	QVector<QOpenGLTexture*> textures;
	VertexLayout vertexLayout;
	QOpenGLVertexArrayObject* pVertexArray{ nullptr };
	QOpenGLShaderProgram* pShader{ nullptr };
};
