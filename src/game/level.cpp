#include "level.h"

Level::Level()
	: m_shader(nullptr)
    , m_wallTexture(nullptr)
    , m_floorTexture(nullptr)
{
	init();
}

Level::~Level()
{
    if (m_floorTexture) {
        m_floorTexture->destroy();
        delete m_floorTexture;
        m_floorTexture = nullptr;
    }

    if (m_wallTexture) {
        m_wallTexture->destroy();
        delete m_wallTexture;
        m_wallTexture = nullptr;
    }

    if (m_shader) {
        m_shader->deleteLater();
    }
}

void Level::init()
{
    m_shader = new QOpenGLShaderProgram();
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertexShader.glsl");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragmentShader.glsl");
    Q_ASSERT(m_shader->link());

    m_wallTexture = new QOpenGLTexture(QImage(":/textures/wall_basecolor.png").mirrored());
    m_wallTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_wallTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_wallTexture->setWrapMode(QOpenGLTexture::ClampToEdge);

    m_floorTexture = new QOpenGLTexture(QImage(":/textures/floor_basecolor.png").mirrored());
    m_floorTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_floorTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_floorTexture->setWrapMode(QOpenGLTexture::ClampToEdge);

    createRoom(4);
    m_actor = std::make_unique<Actor>();
}

void Level::update(const MovementState& movement)
{
    m_actor->update();
    m_actor->onAction(movement, 16.0f);
}

void Level::render()
{
    m_shader->bind();

    /* Light */
    const auto direction = QVector3D(0.4, 0.5, 1.0f);
    m_shader->setUniformValue("uLightBase.direction", direction);
    m_shader->setUniformValue("uLightBase.ambient",   0.5f, 0.5f, 0.5f);
    m_shader->setUniformValue("uLightBase.diffuse",   1.3f, 1.3f, 0.3f);
    m_shader->setUniformValue("uLightBase.specular",  1.0f, 1.0f, 1.0f);
    m_shader->setUniformValue("uLightColor",          1.0f, 1.0f, 1.0f);
    m_shader->setUniformValue("uViewPos",             m_actor->camera()->position());

    /* Camera */
    m_shader->setUniformValue("uProjection", m_projection);
    m_shader->setUniformValue("uView", m_actor->camera()->viewMatrix());

    /* Render objects */
    {
        m_floorTexture->bind();
        for (int i = 0; i < m_floor.size(); i++) {
            m_floor[i]->render();
        }

        m_wallTexture->bind();
        for (int i = 0; i < m_walls.size(); i++) {
            m_walls[i]->render();
        }
    }
}

void Level::createRoom(int size)
{
    for (int i = -size + 2; i <= size + 1; i += 2)
    {
        /* Left */
        m_walls.push_back(std::make_unique<Plain>());
        m_walls.back()->setShader(m_shader);
        m_walls.back()->moveZ(-size);
        m_walls.back()->moveX(i);

        /* Right */
        m_walls.push_back(std::make_unique<Plain>());
        m_walls.back()->setShader(m_shader);
        m_walls.back()->moveZ(size);
        m_walls.back()->moveX(i);

        /* Forward */
        m_walls.push_back(std::make_unique<Plain>());
        m_walls.back()->setShader(m_shader);
        m_walls.back()->rotateY(90);
        m_walls.back()->moveZ(i - 1);
        m_walls.back()->moveX(size + 1);

        /* Back */
        m_walls.push_back(std::make_unique<Plain>());
        m_walls.back()->setShader(m_shader);
        m_walls.back()->rotateY(90);
        m_walls.back()->moveZ(i - 1);
        m_walls.back()->moveX(-size + 1);

        for (int j = 0; j < size * 2; j += 2)
        {
            m_floor.push_back(std::make_unique<Plain>());
            m_floor.back()->setShader(m_shader);
            m_floor.back()->rotateX(-90);
            m_floor.back()->moveY(-1);
            m_floor.back()->moveX(i);
            m_floor.back()->moveZ((-size + 1) + j);
        }
    }
}

void Level::onAxisMove(int dx, int dy)
{
    m_actor->onRotate(dx, dy);
}

void Level::onFramebufferResize(int width, int height)
{
    m_projection.perspective(65, width / float(height), 0.1f, 100.0f);
}
