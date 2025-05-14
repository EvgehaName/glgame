#include "gameobject.h"
#include <qjsonarray.h>

#include <qjsonobject.h>
#include <qtreewidget.h>

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

void GameObject::load(const QJsonObject &config)
{
    auto item = new QTreeWidgetItem;
    setTreeItem(item);

    QString name = config["name"].toString();
    setName(name);

    auto rot = config["rotation"].toArray();
    setRotation(QVector3D(rot[0].toDouble(), rot[1].toDouble(), rot[2].toDouble()));

    auto pos = config["position"].toArray();
    setPosition(QVector3D(pos[0].toDouble(), pos[1].toDouble(), pos[2].toDouble()));

    auto scale = config["scale"].toArray();
    setScale(QVector3D(scale[0].toDouble(), scale[1].toDouble(), scale[2].toDouble()));
}

void GameObject::save(QJsonObject &config)
{

}

void GameObject::setName(const QString &name)
{
    m_name = name;
    if (m_treeItem) {
        m_treeItem->setText(0, m_name);
    }
}

const QString &GameObject::getName() const
{
    return m_name;
}

void GameObject::setPosition(const QVector3D& pos)
{
    m_position = pos;
    m_dirty = true;
}

void GameObject::setRotation(const QVector3D& rot)
{
    m_rotation = rot;
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

void GameObject::setTreeItem(QTreeWidgetItem *item)
{
    if (!item) {
        return;
    }


    /* Сохраним указатель на элемент дерева */
    m_treeItem = item;

    /* Сохраним указатель на себя в элемент дерева */
    m_treeItem->setData(0, Qt::UserRole, QVariant::fromValue<GameObject*>(this));
}
