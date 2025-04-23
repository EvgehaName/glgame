#include "geom.h"

RenderGeometry::RenderGeometry()
{
	pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	pVertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
	Q_ASSERT(pVertexBuffer->create());

	pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	Q_ASSERT(pIndexBuffer->create());

	pVertexArray = new QOpenGLVertexArrayObject();
	Q_ASSERT(pVertexArray->create());
}

void RenderGeometry::render()
{
	vertexLayout.enableAttributes(pShader);
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i]->bind(i);
	}

	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RenderGeometry::insertTexture(QOpenGLTexture* texture, int slot)
{
	Q_ASSERT(texture);
	textures.insert(slot, texture);
}

void RenderGeometry::setShader(QOpenGLShaderProgram* shader)
{
	Q_ASSERT(shader); 
	if (pShader != shader)
	{
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
}

void VertexLayout::addAttribute(GLenum type, GLint size, GLboolean normalized, GLsizei stride, GLsizei offset)
{
	VertexAttribute attr{};
	attr.type        = type;
	attr.size        = size;
	attr.normalized  = normalized;
	attr.stride      = stride;
	attr.offset      = offset;
	
	addAttribute(attr);
}

void VertexLayout::addAttribute(const VertexAttribute& attr)
{
	m_attributes.append(attr);

	switch (attr.type) {
		case GL_FLOAT: 
			m_stride += attr.size * sizeof(GLfloat); 
			break;
		case GL_INT: 
			m_stride += attr.size * sizeof(GLint);
			break;
		case GL_UNSIGNED_INT: 
			m_stride += attr.size * sizeof(GLuint);
			break;
		case GL_SHORT: 
			m_stride += attr.size * sizeof(GLshort);
			break;
		case GL_UNSIGNED_SHORT: 
			m_stride += attr.size * sizeof(GLushort);
			break;
		case GL_BYTE: 
			m_stride += attr.size * sizeof(GLbyte);
			break;
		case GL_UNSIGNED_BYTE: 
			m_stride += attr.size * sizeof(GLubyte);
			break;
	default: break;
	}
}

void VertexLayout::enableAttributes(QOpenGLShaderProgram* program)
{
	Q_ASSERT(program);

	for (int i = 0; i < m_attributes.size(); ++i) {
		const VertexAttribute& attr = m_attributes[i];
		program->enableAttributeArray(i);
		program->setAttributeBuffer(i, attr.type, attr.offset, attr.size, attr.stride);
	}

	m_enabledAttr = true;
}