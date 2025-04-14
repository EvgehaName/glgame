#include "camera/fpscamera.h"

FPSCamera::FPSCamera() {}

void FPSCamera::update()
{
    QVector3D front;
    front.setX(std::cos(m_fYaw) * std::cos(m_fPitch));
    front.setY(std::sin(m_fPitch));
    front.setZ(std::sin(m_fYaw) * std::cos(m_fPitch));

    m_vDirection = front;
    m_vUp = QVector3D::crossProduct(right(), m_vDirection);
    m_vUp.normalize();
}

QMatrix4x4 FPSCamera::viewMatrix()
{
    QMatrix4x4 m;
    m.setToIdentity();
    m.lookAt(m_vPosition, m_vPosition + m_vDirection, m_vUp);

    return m;
}

void FPSCamera::moveCamera(EDirection direction, float dFactor, float deltaTime)
{
    if (direction == EDirection::kNone) {
        return;
    }

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
        case EDirection::kNone:
            break;
        case EDirection::kLeft:
            m_fYaw -= dFactor ? dFactor : (m_vRotSpeed.x() * deltaTime);
            break;
        case EDirection::kRight:
            m_fYaw += dFactor ? dFactor : (m_vRotSpeed.x() * deltaTime);
            break;
        case EDirection::kUp:
            m_fPitch += dFactor ? dFactor : (m_vRotSpeed.y() * deltaTime);
            break;
        case EDirection::kDown:
            m_fPitch -= dFactor ? dFactor : (m_vRotSpeed.y() * deltaTime);
            break;
    }

    if (m_bClampYaw) {
        std::clamp(m_fYaw, m_vLimYaw.x(), m_vLimYaw.y());
    }

    if (m_bClampPitch) {
        std::clamp(m_fPitch, m_vLimPitch.x(), m_vLimPitch.y());
    }
}
