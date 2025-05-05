#include "debugrenderer.h"
#include "dynamics/physics_world.h"

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
    dSpaceID space = PhysicsWorld::getInstance().getSpace();

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
    const dReal* pos = dGeomGetPosition(geomId);
    const dReal* rot = dGeomGetRotation(geomId);

    QMatrix4x4 R;
    R(0, 0) = rot[0]; R(0, 1) = rot[1]; R(0, 2) = rot[2];
    R(1, 0) = rot[4]; R(1, 1) = rot[5]; R(1, 2) = rot[6];
    R(2, 0) = rot[8]; R(2, 1) = rot[9]; R(2, 2) = rot[10];

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(pos[0], pos[1], pos[2]);
    modelMatrix *= R;

    const int type = dGeomGetClass(geomId);
    switch (type) {
        case dBoxClass: {
            dVector3 sides;
            dGeomBoxGetLengths(geomId, sides);
            drawBox(modelMatrix, QVector3D(sides[0], sides[1], sides[2]), Qt::green);
            break;
        }
        case dCapsuleClass: {
            dReal radius, length;
            dGeomCapsuleGetParams(geomId, &radius, &length);
            drawCapsule(modelMatrix, radius, length, Qt::blue);
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

void DebugRenderer::drawCapsule(const QMatrix4x4 &modelMatrix, float radius, float length, const QColor &color)
{
    // 1. Отрисовка цилиндрической части
    drawCylinder(modelMatrix, radius, length, color);

    // 2. Отрисовка верхней полусферы
    QMatrix4x4 topSphereMatrix = modelMatrix;
    topSphereMatrix.translate(0, 0, length / 2);
    drawSphere(topSphereMatrix, radius, color);

    // 3. Отрисовка нижней полусферы
    QMatrix4x4 bottomSphereMatrix = modelMatrix;
    bottomSphereMatrix.translate(0, 0, -length / 2);
    drawSphere(bottomSphereMatrix, radius, color);
}

void DebugRenderer::drawSphere(const QMatrix4x4 &model, float radius, const QColor &color, int segments, int rings)
{
    // Горизонтальные кольца (параллели)
    for (int i = 0; i < rings; ++i) {
        float phi1 = M_PI * i / rings;
        float phi2 = M_PI * (i + 1) / rings;

        float r1 = radius * sin(phi1);
        float z1 = radius * cos(phi1);
        float r2 = radius * sin(phi2);
        float z2 = radius * cos(phi2);

        for (int j = 0; j < segments; ++j) {
            float theta1 = 2.0f * M_PI * j / segments;
            float theta2 = 2.0f * M_PI * (j + 1) / segments;

            float x11 = r1 * cos(theta1);
            float y11 = r1 * sin(theta1);
            float x12 = r1 * cos(theta2);
            float y12 = r1 * sin(theta2);

            float x21 = r2 * cos(theta1);
            float y21 = r2 * sin(theta1);
            float x22 = r2 * cos(theta2);
            float y22 = r2 * sin(theta2);

            QVector3D p11(x11, y11, z1);
            QVector3D p12(x12, y12, z1);
            QVector3D p21(x21, y21, z2);
            QVector3D p22(x22, y22, z2);

            // Преобразуем точки в мировые координаты
            p11 = model * p11;
            p12 = model * p12;
            p21 = model * p21;
            p22 = model * p22;

            // Рисуем линии вдоль параллелей
            drawLine(p11, p12, color);

            // Рисуем линии вдоль меридианов
            if (i < rings - 1) {
                drawLine(p11, p21, color);
            }
        }
    }
}

void DebugRenderer::drawCylinder(const QMatrix4x4 &model, float radius, float height, const QColor &color, int segments)
{
    const float halfHeight = height / 2.0f;

    // Боковая поверхность (вертикальные линии)
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        float x1 = radius * cos(angle1);
        float y1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float y2 = radius * sin(angle2);

        QVector3D bottom1(x1, y1, -halfHeight);
        QVector3D top1(x1, y1, halfHeight);
        QVector3D bottom2(x2, y2, -halfHeight);
        QVector3D top2(x2, y2, halfHeight);

        // Преобразуем точки в мировые координаты
        bottom1 = model * bottom1;
        top1 = model * top1;
        bottom2 = model * bottom2;
        top2 = model * top2;

        // Рисуем вертикальные линии
        drawLine(bottom1, top1, color);

        // Рисуем нижнее и верхнее кольцо
        drawLine(bottom1, bottom2, color);
        drawLine(top1, top2, color);
    }
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
