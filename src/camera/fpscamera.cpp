#include "camera/fpscamera.h"

FPSCamera::FPSCamera() {}

QMatrix4x4 FPSCamera::viewMatrix()
{
    QMatrix4x4 m;
    m.setToIdentity();
    m.lookAt(m_vPosition, m_vPosition + m_vDirection, m_vUp);

    return m;
}

void FPSCamera::update(const QVector3D &cameraPosition)
{
    m_vPosition = cameraPosition;

    QVector3D front;
    front.setX(std::cos(m_fYaw) * std::cos(m_fPitch));
    front.setY(std::sin(m_fPitch));
    front.setZ(std::sin(m_fYaw) * std::cos(m_fPitch));

    m_vDirection = front;
    m_vUp = QVector3D::crossProduct(right(), m_vDirection);
    m_vUp.normalize();
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
        case EInputScreenDirection::Left:
            m_fYaw -= dFactor ? dFactor : (m_vRotSpeed.x() * deltaTime);
            break;
        case EInputScreenDirection::Right:
            m_fYaw += dFactor ? dFactor : (m_vRotSpeed.x() * deltaTime);
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
