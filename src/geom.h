#ifndef GEOM_H
#define GEOM_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>

class Geom
{
    public: 
        QOpenGLShaderProgram *m_programShader;
    protected:
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;

        const float verticesPlace[32] = {
            //x    //y   //z    //normal          // uv
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // нижняя правая точка
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // верняя правая точка
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // верхняя левая точка
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f // нижняя левая точка
        };    
    
        const GLuint indicesPlace[6] = {
            0, 3, 1, 3, 2, 1  // квадрат
        };
};

#endif // GEOM_H
