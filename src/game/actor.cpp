#include "actor.h"

#include <QDebug>
#include <cmath>
#include <QApplication>

Actor::Actor()
{
    QString setpath = QApplication::applicationDirPath() + "/config.ini";
    QSettings settings(setpath, QSettings::NativeFormat);
    m_camera.loadSection("actor_cam", settings);
}

void Actor::onAction(const MovementState& state, float deltaTime)
{
    QVector3D vAccel(0.f, 0.f, 0.f);

    const float speed = 0.0025f;

    if (state.m_forward) {
        vAccel += { 0.0f, 0.0f, 1.0f };
    }

    if (state.m_back) {
        vAccel -= { 0.0f, 0.0f, 1.0f };
    }

    if (state.m_left) {
        vAccel -= { 1.0f, 0.0f, 0.0f };
    }

    if (state.m_right) {
        vAccel += { 1.0f, 0.0f, 0.0f };
    }

    vAccel.normalize();

    QMatrix4x4 R;
    R.setToIdentity();
    R.rotate(qRadiansToDegrees(m_camera.yaw()), {0.0f, 1.0f, 0.0f});

#if QT_VERSION >= 0x060000
    vAccel = R.map(vAccel);
#else
    vAccel = R * vAccel;
#endif
    m_position += vAccel * speed * deltaTime;
}

void Actor::onRotate(int dx, int dy)
{
    float fx = static_cast<float>(dx);
    float fy = static_cast<float>(dy);

    const float scale = (m_camera.fov() / m_baseFov) * m_mouseSens * m_mouseSensScale / 50.f / m_lookFactor;
    constexpr float eps = 0.0000001f;

    if (std::abs(fx) > eps) {
        const float dFactor = hDirectionFactor(fx, scale, true);
        EInputScreenDirection dType = toScreenDirection(EInputAxis::Horizontal, dFactor);
        m_camera.moveCamera(dType, std::abs(dFactor), 16.f);
    }

    if (std::abs(fy) > eps) {
        float dFactor = vDirectionFactor(fy, scale, false);
        EInputScreenDirection dType = toScreenDirection(EInputAxis::Vertical, dFactor);
        m_camera.moveCamera(dType, std::abs(dFactor), 16.f);
    }
}

void Actor::update()
{
    m_camera.update(m_position);
}

CameraBase *Actor::camera()
{
    return &m_camera;
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
