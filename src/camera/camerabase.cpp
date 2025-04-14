#include "camera/camerabase.h"

CameraBase::CameraBase()
{
    m_vUp.setX(0.f); m_vUp.setY(1.f); m_vUp.setZ(0.f);
    m_vWorldUp.setX(0.f); m_vWorldUp.setY(1.f); m_vWorldUp.setZ(0.f);

    m_vPosition.setX(0.f); m_vPosition.setY(0.f); m_vPosition.setZ(0.f);
    m_vDirection.setX(0.f); m_vDirection.setY(0.f); m_vDirection.setZ(1.f);

    m_fYaw = 0.f; m_fPitch = 0.f; m_fRoll = 0.f;
    m_fFov = 90.f; m_fAspect = 1.f;
    m_bClampYaw = false; m_bClampRoll = false; m_bClampPitch = false;

    m_vLimYaw.setX(0.f); m_vLimYaw.setY(0.f);
    m_vLimRoll.setX(0.f); m_vLimRoll.setY(0.f);
    m_vLimPitch.setX(0.f); m_vLimPitch.setY(0.f);
    m_vRotSpeed.setX(3.14f); m_vRotSpeed.setY(3.14f); m_vRotSpeed.setZ(0.0f);
}

void CameraBase::loadSection(const QString &section, QSettings& settings)
{
    settings.beginGroup(section);
    {
        m_vRotSpeed.setX(settings.value("rot_speed_x", 3.14f).toFloat());
        m_vRotSpeed.setY(settings.value("rot_speed_y", 3.14f).toFloat());
        m_vRotSpeed.setZ(settings.value("rot_speed_z", 0).toFloat());

        m_vLimYaw.setX(settings.value("lim_yaw_min", 0).toFloat());
        m_vLimYaw.setY(settings.value("lim_yaw_max", 0).toFloat());

        m_vLimPitch.setX(settings.value("lim_pitch_min", -1.5f).toFloat());
        m_vLimPitch.setY(settings.value("lim_pitch_max", 1.5f).toFloat());

        m_bClampYaw = (0 != m_vLimYaw.x()) || (0 != m_vLimYaw.y());
        m_bClampPitch = (0 != m_vLimPitch.x()) || (0 != m_vLimPitch.y());

        if (m_bClampYaw) {
            m_fYaw = (m_vLimYaw.x() + m_vLimYaw.y()) * 0.5f;
        }

        if (m_bClampPitch) {
            m_fPitch = (m_vLimPitch.x() + m_vLimPitch.y()) * 0.5f;
        }
    }
    settings.endGroup();
}
