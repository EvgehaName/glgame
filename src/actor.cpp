#include "actor.h"

#include <QDebug>
#include <cmath>
#include <QApplication>

Actor::Actor()
{
    QString setpath = QApplication::applicationDirPath() + "/config.ini";
    QSettings settings(setpath, QSettings::NativeFormat);

    m_camera = new FPSCamera();
    m_camera->loadSection("actor_cam", settings);
}

void Actor::onAction(EMovementAction action, float deltaTime)
{
    const float acceleration = 0.005f;
    const float coef = acceleration * deltaTime;

    switch (action)
    {
        case EMovementAction::kForwardStrafe:
            m_position += m_camera->direction() * coef;
            break;
        case EMovementAction::kBackStrafe:
            m_position -= m_camera->direction() * coef;
            break;
        case EMovementAction::kLeftStrafe:
            m_position += m_camera->right()     * coef;
            break;
        case EMovementAction::kRightStrafe:
            m_position -= m_camera->right()     * coef;
            break;
    }
}

void Actor::onRotate(int dx, int dy)
{
    float fx = static_cast<float>(dx);
    float fy = static_cast<float>(dy);

    const float scale = (camera()->fov() / m_baseFov) * m_mouseSens * m_mouseSensScale / 50.f / m_lookFactor;

    if (fx != 0) {
        const float dFactor = hDirectionFactor(fx, scale, false);
        EInputScreenDirection dType = toScreenDirection(EInputAxis::Horizontal, dFactor);
        camera()->moveCamera(dType, std::fabs(dFactor), 16.f);
    }

    if (fy != 0) {
        float dFactor = vDirectionFactor(fy, scale, false);
        EInputScreenDirection dType = toScreenDirection(EInputAxis::Vertical, dFactor);
        camera()->moveCamera(dType, std::fabs(dFactor), 16.f);
    }
}

void Actor::update()
{
    camera()->update(m_position);
}

CameraBase *Actor::camera()
{
    return m_camera;
}

float Actor::hDirectionFactor(float x, float scaleX, bool invertX) const
{
    float factor = (invertX ? -1.f : 1.f) * x * scaleX;
    return factor;
}

float Actor::vDirectionFactor(float y, float scaleY, bool invertY) const
{
    float factor = (invertY ? -1.f : 1.f) * y * scaleY * 3.f / 4.f;
    return factor;
}

EInputScreenDirection Actor::toScreenDirection(EInputAxis axis, float value) const
{
    switch (axis) {
        case EInputAxis::Vertical:
            return (value > 0) ? EInputScreenDirection::Up : EInputScreenDirection::Down;
        case EInputAxis::Horizontal:
            return (value < 0) ? EInputScreenDirection::Left : EInputScreenDirection::Right;
        default:
            Q_ASSERT_X(false, Q_FUNC_INFO, "Unknown input axis");
    }

    Q_UNREACHABLE();
}
