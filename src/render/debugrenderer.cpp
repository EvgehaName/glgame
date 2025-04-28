#include "debugrenderer.h"
#include "core/engine.h"

DebugRenderer::DebugRenderer()
    : m_initialized(false)
{
    m_vertexArray = new QOpenGLVertexArrayObject();
    Q_ASSERT(m_vertexArray->create());
    m_vertexArray->bind();

    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    Q_ASSERT(m_vertexBuffer->create());
    Q_ASSERT(m_vertexBuffer->bind());

    initShaders();
    m_initialized = true;
}

DebugRenderer::~DebugRenderer()
{
    if (m_initialized)
    {
        m_vertexArray->destroy();
        delete m_vertexArray;

        m_vertexBuffer->destroy();
        delete m_vertexBuffer;

        m_shader->removeAllShaders();
        delete m_shader;
    }

    m_shader = nullptr;
    m_vertexArray = nullptr;
    m_vertexBuffer = nullptr;
}

void DebugRenderer::drawAllGeom()
{
    dSpaceID space = Engine::get().space();

    int geoms = dSpaceGetNumGeoms(space);
    if (geoms > 0)
    {
        for (int i = 0; i < geoms; i++)
        {
            dGeomID geom = dSpaceGetGeom(space, i);
            drawGeom(geom);
        }
    }
}

void DebugRenderer::drawGeom(dGeomID geomId)
{
    const int type = dGeomGetClass(geomId);

    const dReal* pos = dGeomGetPosition(geomId);
    const dReal* rot = dGeomGetRotation(geomId);

    QMatrix4x4 R;
    R(0, 0) = rot[0]; R(0, 1) = rot[1]; R(0, 2) = rot[2];
    R(1, 0) = rot[4]; R(1, 1) = rot[5]; R(1, 2) = rot[6];
    R(2, 0) = rot[8]; R(2, 1) = rot[9]; R(2, 2) = rot[10];

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pos[0], pos[1], pos[2]);
    modelMatrix *= R;

    switch (type) {
        case dBoxClass: {
            dVector3 sides;
            dGeomBoxGetLengths(geomId, sides);
            drawBox(modelMatrix, QVector3D(sides[0], sides[1], sides[2]), Qt::green);
            break;
        }
        default:
            break;
    }
}

void DebugRenderer::drawLine(const QVector3D &start, const QVector3D &end, const QColor &color)
{
    addVertex(start, color);
    addVertex(end, color);
}

void DebugRenderer::drawBox(const QMatrix4x4 &model, const QVector3D &sides, const QColor &color)
{
    const QVector3D halfSides = sides * 0.5f;

    const QVector3D localVertices[8] = {
        { -halfSides.x(), -halfSides.y(), -halfSides.z() },
        {  halfSides.x(), -halfSides.y(), -halfSides.z() },
        {  halfSides.x(),  halfSides.y(), -halfSides.z() },
        { -halfSides.x(),  halfSides.y(), -halfSides.z() },
        { -halfSides.x(), -halfSides.y(),  halfSides.z() },
        {  halfSides.x(), -halfSides.y(),  halfSides.z() },
        {  halfSides.x(),  halfSides.y(),  halfSides.z() },
        { -halfSides.x(),  halfSides.y(),  halfSides.z() }
    };

    QVector3D worldVertices[8];
    for (int i = 0; i < 8; ++i) {
        worldVertices[i] = model * localVertices[i];
    }

    /* Down */
    drawLine(worldVertices[0], worldVertices[1], color);
    drawLine(worldVertices[1], worldVertices[2], color);
    drawLine(worldVertices[2], worldVertices[3], color);
    drawLine(worldVertices[3], worldVertices[0], color);

    /* Up */
    drawLine(worldVertices[4], worldVertices[5], color);
    drawLine(worldVertices[5], worldVertices[6], color);
    drawLine(worldVertices[6], worldVertices[7], color);
    drawLine(worldVertices[7], worldVertices[4], color);

    /* Sides */
    drawLine(worldVertices[0], worldVertices[4], color);
    drawLine(worldVertices[1], worldVertices[5], color);
    drawLine(worldVertices[2], worldVertices[6], color);
    drawLine(worldVertices[3], worldVertices[7], color);
}

void DebugRenderer::render(const QMatrix4x4 &viewProjectionMatrix)
{
    if (m_vertices.empty() || !m_initialized) {
        return;
    }

    m_shader->bind();
    m_shader->setUniformValue("uViewProjection", viewProjectionMatrix);

    m_vertexArray->bind();
    m_vertexBuffer->bind();
    m_vertexBuffer->allocate(m_vertices.constData(), static_cast<int>(m_vertices.size() * sizeof(Vertex)));

    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size()));
}

void DebugRenderer::clear()
{
    m_vertices.clear();
}

void DebugRenderer::initShaders()
{
    m_shader = new QOpenGLShaderProgram();

    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "uniform mat4 uViewProjection;\n"
        "out vec4 vColor;\n"
        "void main() {\n"
        "    gl_Position = uViewProjection * vec4(position, 1.0);\n"
        "    vColor = color;\n"
        "}\n";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "in vec4 vColor;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    fragColor = vColor;\n"
        "}\n";

    Q_ASSERT(m_shader->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource));
    Q_ASSERT(m_shader->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource));
    if (!m_shader->link()) {
        qWarning() << "Failed to link shader program:" << m_shader->log();
        Q_ASSERT(true);
    }

    m_shader->enableAttributeArray(0);
    m_shader->setAttributeBuffer(0, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));

    m_shader->enableAttributeArray(1);
    m_shader->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, color), 4, sizeof(Vertex));
}

void DebugRenderer::addVertex(const QVector3D &position, const QColor &color)
{
    QVector4D vColor;
    vColor.setX(color.red());
    vColor.setY(color.green());
    vColor.setZ(color.blue());
    vColor.setW(color.alpha());

    m_vertices.append({position, vColor});
}
