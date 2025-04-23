#include "gameobject.h"

GameObject::GameObject()
{
    m_programShader = new QOpenGLShaderProgram();
    m_programShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertexShader.glsl");
    m_programShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragmentShader.glsl");
    initBuffersPlaceObject();
}

void GameObject::draw()
{
    
}

void GameObject::initTexture()
{
    textureMap["wall"] = new QOpenGLTexture(QImage(":/textures/wall_basecolor.png").mirrored());
    textureMap["wall"]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureMap["wall"]->setMagnificationFilter(QOpenGLTexture::Linear);
    textureMap["wall"]->setWrapMode(QOpenGLTexture::Repeat);

    textureMap["floor"] = new QOpenGLTexture(QImage(":/textures/floor_basecolor.png").mirrored());
    textureMap["floor"]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureMap["floor"]->setMagnificationFilter(QOpenGLTexture::Linear);
    textureMap["floor"]->setWrapMode(QOpenGLTexture::Repeat);
}

void GameObject::initBuffersPlaceObject()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlace), verticesPlace, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPlace), indicesPlace, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
