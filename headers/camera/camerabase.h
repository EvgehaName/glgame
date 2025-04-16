#ifndef CAMERABASE_H
#define CAMERABASE_H

#include <QtMath>
#include <QSettings>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>

#include "actions.h"
#include "constants.h"

class CameraBase
{
public:
    CameraBase();

    const float& fov() const { return m_fFov; }
    const float& aspect() const { return m_fAspect; }

    const QVector3D& position() const { return m_vPosition; }
    const QVector3D& direction() const { return m_vDirection; }
    const QVector3D& up() const { return m_vUp; }
    QVector3D right() const { return QVector3D::crossProduct(m_vDirection, m_vWorldUp); }

    virtual void loadSection(const QString& section, QSettings& settings);

    virtual QMatrix4x4 viewMatrix() = 0;
    virtual void update(const QVector3D& cameraPosition) = 0;
    virtual void moveCamera(EDirection direction, float dFactor, float deltaTime) = 0;

protected:
    QVector3D m_vUp;
    QVector3D m_vWorldUp;
    QVector3D m_vPosition;
    QVector3D m_vDirection;

    QVector2D m_vLimYaw, m_vLimPitch, m_vLimRoll;
    QVector3D m_vRotSpeed;

    float m_fFov;
    float m_fAspect;
    float m_fYaw, m_fPitch, m_fRoll;

    bool m_bClampYaw;
    bool m_bClampRoll;
    bool m_bClampPitch;
};

#endif // CAMERABASE_H
