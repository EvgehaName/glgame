#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglfunctions.h>
#include <qopenglbuffer.h>
#include <qmatrix4x4.h>
#include <qvector3d.h>
#include <qvector.h>
#include <qcolor.h>

#include <ode/ode.h>

class DebugRenderer
{
public:
    struct Vertex {
        QVector3D position;
        QVector4D color;
    };

    DebugRenderer();
    ~DebugRenderer();

    void drawAllGeom();
    void drawGeom(dGeomID geomId);
    void drawLine(const QVector3D& start, const QVector3D& end, const QColor& color);
    void drawBox(const QMatrix4x4& model, const QVector3D& sides, const QColor& color);
    void drawCapsule(const QMatrix4x4 &model, float radius, float length, const QColor &color);
    void drawSphere(const QMatrix4x4 &model, float radius, const QColor &color, int segments = 16, int rings = 8);
    void drawCylinder(const QMatrix4x4 &model, float radius, float height, const QColor &color, int segments = 16);

    void render(const QMatrix4x4& viewProjectionMatrix);
    void clear();

private:
    QOpenGLVertexArrayObject* m_vertexArray;
    QOpenGLShaderProgram* m_shader;
    QOpenGLBuffer* m_vertexBuffer;
    QVector<Vertex> m_vertices;
    bool m_initialized;

    void initShaders();
    void addVertex(const QVector3D& position, const QColor& color);
};

#endif // DEBUGRENDERER_H
