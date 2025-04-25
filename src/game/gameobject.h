#pragma once
#include "../render/geom.h"

class GameObject
{
public:    
    GameObject();

    void moveX(float offset);
    void moveY(float offset);
    void moveZ(float offset);
    void move(const QVector3D& offset);
    void setPosition(const QVector3D& pos);

    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void rotate(const QVector3D& rot);
    void setRotation(const QVector3D& rot);

    void scaleX(float factor);
    void scaleY(float factor);
    void scaleZ(float factor);
    void scale(float factor);
    void scale(const QVector3D& scale);
    void setScale(const QVector3D& scale);

    const QMatrix4x4& getModelMatrix() const;

protected:
    QVector3D m_scale;
    QVector3D m_rotation;
    QVector3D m_position;
    mutable bool m_dirty;
    mutable QMatrix4x4 m_modelMatrix;
};