#include "level.h"

Level::Level()
	: m_floor(nullptr)
    , m_shader(nullptr)
    , m_wallTexture(nullptr)
    , m_floorTexture(nullptr)
{
	for (int i = 0; i < 4; i++) {
		m_walls[i] = nullptr;
	}

	init();
}

Level::~Level()
{
    if (m_actor) {
        delete m_actor;
        m_actor = nullptr;
    }

    if (m_floor) {
        delete m_floor;
        m_floor = nullptr;
    }

    for (int i = 0; i < 4; i++) {
        if (m_walls[i]) {
            delete m_walls[i];
            m_walls[i] = nullptr;
        }
    }

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
    m_wallTexture->setWrapMode(QOpenGLTexture::Repeat);

    m_floorTexture = new QOpenGLTexture(QImage(":/textures/floor_basecolor.png").mirrored());
    m_floorTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_floorTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_floorTexture->setWrapMode(QOpenGLTexture::Repeat);

    const QVector3D worldUP(0.0f, 1.0f, 0.0f);
    const QVector3D worldRight(1.0f, 0.0f, 0.0f);

    m_floor = new Plain();
    m_floor->setShader(m_shader);
    //m_floor->rotate(90, worldRight);

    for (int i = 0; i < 4; i++) {
        m_walls[i] = new Plain();
        m_walls[i]->setShader(m_shader);
    }

    m_walls[0]->moveZ(-0.5);
    m_walls[1]->moveZ(0.5);

    m_walls[2]->rotateY(90);
    m_walls[3]->rotateY(-90);

    ///* Left */
    //m_walls[0]->rotateY(90);
    //m_walls[0]->moveX(-0.5);

    ///* Right */
    //m_walls[1]->rotateY(-90);
    //m_walls[1]->moveX(0.5);

    ///* «адн€€ стена (Z-) */
    //m_walls[2]->moveZ(0.5);     // —двигаем назад

    /* ѕередн€€ стена (Z+) Ч если нужна */
    //m_walls[3]->moveZ(0.5);      // —двигаем вперЄд

    ///* Left */
    //m_walls[0]->moveZ(-3);
    //m_walls[0]->scaleX(3);

    ///* Right */
    //m_walls[1]->scaleX(3);
    //m_walls[1]->moveZ(3);

    ///* Back */
    //m_walls[2]->moveZ(-1.5);
    //m_walls[2]->rotateY(90);
    //m_walls[2]->scaleX(5.5);

    /* Forward */
    m_walls[3]->moveZ(2);
  //  m_walls[3]->rotateY(90);

    m_actor = new Actor();
}

void Level::update(const MovementState& movement)
{
    m_actor->update();
    m_actor->onAction(movement, 16.0f);
}

void Level::render()
{
    m_shader->bind();

    QMatrix4x4 view = m_actor->camera()->viewMatrix();

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
    m_shader->setUniformValue("uView",       view);

    /* Render objects */
    {
        m_floorTexture->bind();
       // m_floor->render();

        m_wallTexture->bind();
        for (int i = 0; i < 4; i++) {
            m_walls[i]->render();
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
