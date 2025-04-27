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

    m_actor = std::make_unique<Actor>(this);
}

void Level::update(const MovementState& movement)
{
    m_actor->onAction(movement, 16.0f);
    m_actor->update();
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

    m_wallTexture->bind();
    for (int i = 0; i < m_objects.size(); i++) 
    {
        GameObject* pGameObject = m_objects[i];
        const QMatrix4x4& model = pGameObject->getModelMatrix();

        RenderGeometry* pRenderable = dynamic_cast<RenderGeometry*>(pGameObject);
        pRenderable->shader()->setUniformValue("uModel", model);
        pRenderable->render();
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

Actor *Level::actor()
{
    return m_actor.get();
}

void Level::addGameObject(GameObject* ptr)
{
    Q_ASSERT(ptr);
    RenderGeometry* pGeom = dynamic_cast<RenderGeometry*>(ptr);
    pGeom->setShader(m_shader);
    m_objects.push_back(ptr);
}

void Level::add_collision_object(collision::collision_object* collision_ptr)
{
    Q_ASSERT(collision_ptr);
    m_collisions.push_back(collision_ptr);
}

QMatrix4x4 Level::getViewProjection() const
{
    QMatrix4x4 view = m_actor->camera()->viewMatrix();
    return m_projection * view;
}

bool Level::check_collide(collision::collision_object* who_collision_ptr)
{
    for (int i = 0; i < m_collisions.size(); i++) {
        if (who_collision_ptr->check_collide(m_collisions[i])) {
            return true;
        }
    }

    return false;
}
