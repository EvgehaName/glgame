#include "gameobject.h"
#include <qjsonarray.h>

GameObject::GameObject()
    : m_scale(1.0f, 1.0f, 1.0f)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_position(0.0f, 0.0f, 0.0f)
    , m_dirty(true)
{
    m_modelMatrix.setToIdentity();
}

GameObject::GameObject(const QJsonArray& pos, const QJsonArray& rot, const QJsonArray& scl)
{
    m_position.setX(pos[0].toDouble());
    m_position.setY(pos[1].toDouble());
    m_position.setZ(pos[2].toDouble());

    m_rotation.setX(rot[0].toDouble());
    m_rotation.setY(rot[1].toDouble());
    m_rotation.setZ(rot[2].toDouble());
    
    m_scale.setX(scl[0].toDouble());
    m_scale.setY(scl[1].toDouble());
    m_scale.setZ(scl[2].toDouble());
}

void GameObject::moveX(float offset)
{
    move({ offset, 0.0f, 0.0f });
}

void GameObject::moveY(float offset)
{
    move({ 0.0f, offset, 0.0f });
}

void GameObject::moveZ(float offset)
{
    move({ 0.0f, 0.0f, offset });
}

void GameObject::move(const QVector3D& offset)
{
    m_position += offset;
    m_dirty = true;
}

void GameObject::setPosition(const QVector3D& pos)
{
    m_position = pos;
    m_dirty = true;
}

void GameObject::rotateX(float angle)
{
    rotate({ angle, 0.0f, 0.0f });
}

void GameObject::rotateY(float angle)
{
    rotate({ 0.0f, angle, 0.0f });
}

void GameObject::rotateZ(float angle)
{
    rotate({ 0.0f, 0.0f, angle });
}

void GameObject::rotate(const QVector3D& rot)
{
    m_rotation += rot;

    m_dirty = true;
}

void GameObject::setRotation(const QVector3D& rot)
{
    m_rotation = rot;
    m_dirty = true;
}

void GameObject::scaleX(float factor)
{
    scale({ factor, 0.0f, 0.0f });
}

void GameObject::scaleY(float factor)
{
    scale({ 0.0f, factor, 0.0f });
}

void GameObject::scaleZ(float factor)
{
    scale({ 0.0f, 0.0f, factor });
}

void GameObject::scale(float factor)
{
    scale({ factor, factor, factor });
}

void GameObject::scale(const QVector3D& scale)
{
    m_scale += scale;
    m_dirty = true;
}

void GameObject::setScale(const QVector3D& scale)
{
    m_scale = scale;
    m_dirty = true;
}

const QMatrix4x4& GameObject::getModelMatrix() const
{
    if (m_dirty)
    {
        QQuaternion Q = QQuaternion::fromEulerAngles(m_rotation);

        m_modelMatrix.setToIdentity();
        m_modelMatrix.translate(m_position);
        m_modelMatrix.rotate(Q);
        m_modelMatrix.scale(m_scale);

        m_dirty = false;
    }

    return m_modelMatrix;
}
