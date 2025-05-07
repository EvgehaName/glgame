#pragma once
#include "../render/geom.h"

#include <QObject>

class QTreeWidgetItem;

class GameObject : public QObject
{
public:    
    GameObject();

    GameObject(const QJsonArray& pos, const QJsonArray& rot, const QJsonArray& scl);

    virtual void load(const QJsonObject& config);
    virtual void save(QJsonObject& config);
    virtual void render() {}

    void setName(const QString& name);
    const QString& getName() const;

    void setPosition(const QVector3D& pos);
    const QVector3D& getPosition() const { return m_position; }
    void setRotation(const QVector3D& rot);
    const QVector3D& getRotation() const { return m_rotation; }
    void setScale(const QVector3D& scale);
    const QVector3D& getScale() const { return m_scale; }

    // TODO: FIX ME (ITS FOR POLYMORPHIC TYPE)
    virtual void what() = 0;
    const QMatrix4x4& getModelMatrix() const;

    /* EDITOR MODE ONLY */
    inline void setTreeItem(QTreeWidgetItem* item) { m_treeItem = item; }
    QTreeWidgetItem* treeItem() const { return m_treeItem; }

protected:

    /* ATTRIBUTES */
    QString   m_name;
    QVector3D m_scale;
    QVector3D m_rotation;
    QVector3D m_position;

    /* STATE */
    mutable bool       m_dirty{ false };
    mutable QMatrix4x4 m_modelMatrix;

    /* EDITOR MODE ONLY */
    QTreeWidgetItem* m_treeItem{nullptr};
};
