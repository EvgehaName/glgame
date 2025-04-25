#include "fpscamera.h"

FPSCamera::FPSCamera() {}

QMatrix4x4 FPSCamera::viewMatrix()
{
    QMatrix4x4 m;
    m.setToIdentity();
    m.lookAt(m_vPosition, m_vPosition + m_vDirection, m_vUp);

    return m;
}

void FPSCamera::update(const QVector3D &position)
{
    m_vPosition = position;

    QMatrix4x4 M;
    M.setToIdentity();

    QQuaternion R = QQuaternion::fromEulerAngles(qRadiansToDegrees(m_fPitch), qRadiansToDegrees(m_fYaw), qRadiansToDegrees(m_fRoll));
    M.rotate(R);

    m_vDirection = M.column(2).toVector3D();
    m_vUp        = M.column(1).toVector3D();
}

void FPSCamera::moveCamera(EInputScreenDirection direction, float dFactor, float deltaTime)
{
    if (m_bClampPitch)
    {
        while (m_fPitch < m_vLimPitch.x()) {
            m_fPitch += M_2PI;
        }

        while (m_fPitch > m_vLimPitch.y()) {
            m_fPitch -= M_2PI;
        }
    };

    switch (direction) {
        case EInputScreenDirection::Right:
            m_fYaw += dFactor ? dFactor : (m_vRotSpeed.x() * deltaTime);
            break;
        case EInputScreenDirection::Left:
            m_fYaw -= dFactor ? dFactor : (m_vRotSpeed.x() * deltaTime);
            break;
        case EInputScreenDirection::Up:
            m_fPitch += dFactor ? dFactor : (m_vRotSpeed.y() * deltaTime);
            break;
        case EInputScreenDirection::Down:
            m_fPitch -= dFactor ? dFactor : (m_vRotSpeed.y() * deltaTime);
            break;
    }

    if (m_bClampYaw) {
        m_fYaw = std::clamp(m_fYaw, m_vLimYaw.x(), m_vLimYaw.y());
    }

    if (m_bClampPitch) {
        m_fPitch = std::clamp(m_fPitch, m_vLimPitch.x(), m_vLimPitch.y());
    }
}
