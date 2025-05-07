#include "level.h"
#include "dynamics/collision/collision_factory.h"

#include <qfile.h>
#include <qfileinfo.h>
#include <qtreewidget.h>

#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonparseerror.h>

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
    if (!m_shader->link()) {
        qFatal("Failed to link shader program");
    }

    m_wallTexture = new QOpenGLTexture(QImage(":/textures/wall_basecolor.png").mirrored());
    m_wallTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_wallTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_wallTexture->setWrapMode(QOpenGLTexture::ClampToEdge);

    m_floorTexture = new QOpenGLTexture(QImage(":/textures/floor_basecolor.png").mirrored());
    m_floorTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_floorTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_floorTexture->setWrapMode(QOpenGLTexture::ClampToEdge);

    m_actor = std::make_unique<Actor>();
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
    auto direction = QVector3D(1.2f, 0.7f, 0.0f);
    direction.normalize();

    m_shader->setUniformValue("uLightBase.direction", direction);
    m_shader->setUniformValue("uLightBase.ambient",   0.5f, 0.5f, 0.5f);
    m_shader->setUniformValue("uLightBase.diffuse",   0.3f, 0.3f, 0.3f);
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

#ifdef QT_DEBUG
        m_dbgRender->drawAllGeom();
        m_dbgRender->render(getViewProjection());
        m_dbgRender->clear();
#endif // QT_DEBUG
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

    /* EDITOR MODE ONLY */
    if (m_rootItem && ptr->treeItem()) {
        m_rootItem->addChild(ptr->treeItem());
    }
}

void Level::add_collision_object(Collision* collision_ptr)
{
    Q_ASSERT(collision_ptr);
    m_collisions.push_back(collision_ptr);
}

QMatrix4x4 Level::getViewProjection() const
{
    QMatrix4x4 view = m_actor->camera()->viewMatrix();
    return m_projection * view;
}

bool Level::load(const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "An error occurred while opening the level file:" << file.errorString();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << Q_FUNC_INFO << "An error occurred while reading the project file - " << parseError.offset << ":" << parseError.errorString();
        return false;
    }

    QFileInfo finfo(filepath);
    m_rootItem = new QTreeWidgetItem();
    m_rootItem->setText(0, finfo.baseName());

    QJsonObject root = document.object();
    load(root);

    return true;
}

void Level::load(const QJsonObject& config)
{
    QJsonArray objectsArray = config["objects"].toArray();
    for (const auto &objectValue : std::as_const(objectsArray))
    {
        GameObject* pObject = new Plain();
        pObject->load(objectValue.toObject());
        addGameObject(pObject);
    }

    QJsonArray collisionsArray = config["collision_objects"].toArray();
    for (const auto &objectValue : std::as_const(collisionsArray))
    {
        QJsonObject config = objectValue.toObject();
        Collision* pCollision = CollisionFactory::createCollision(intToCollisionType(config["type"].toInt()));
        if (pCollision) {
            pCollision->load(config);
            add_collision_object(pCollision);
        } else {
            qWarning() << Q_FUNC_INFO << "Unsupported collision type";
        }
    }

    onLevelLoaded();
}

void Level::onLevelLoaded()
{
    m_dbgRender = new DebugRenderer();
    m_dbgRender->drawAllGeom();
}
