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
    QVector3D vAccel(0.f, 0.f, 0.f);

    const float speed = 0.005f;
    // const float coef = acceleration * deltaTime;

    // TODO: action bit mask
    if (action == EMovementAction::kForwardStrafe) {
        vAccel += { 0.0f, 0.0f, 1.0f };
    }
    if (action == EMovementAction::kBackStrafe) {
        vAccel -= { 0.0f, 0.0f, 1.0f };
    }
    if (action == EMovementAction::kLeftStrafe) {
        vAccel -= { 1.0f, 0.0f, 0.0f };
    }
    if (action == EMovementAction::kRightStrafe) {
        vAccel += { 1.0f, 0.0f, 0.0f };
    }

    // TODO: add orientation
    m_position += vAccel * speed * deltaTime;
}

void Actor::onRotate(int dx, int dy)
{
    float fx = static_cast<float>(dx);
    float fy = static_cast<float>(dy);

    const float scale = (camera()->fov() / m_baseFov) * m_mouseSens * m_mouseSensScale / 50.f / m_lookFactor;
    constexpr float eps = 0.0000001f;

    if (std::abs(fx) > eps) {
        const float dFactor = hDirectionFactor(fx, scale, true);
        EInputScreenDirection dType = toScreenDirection(EInputAxis::Horizontal, dFactor);
        camera()->moveCamera(dType, std::abs(dFactor), 16.f);
    }

    if (std::abs(fy) > eps) {
        float dFactor = vDirectionFactor(fy, scale, false);
        EInputScreenDirection dType = toScreenDirection(EInputAxis::Vertical, dFactor);
        camera()->moveCamera(dType, std::abs(dFactor), 16.f);
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
