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

void Actor::onAction(EGameAction action)
{
    switch (action)
    {
        case EGameAction::kForwardStrafe:
            m_position += { 0.0f, 0.0f, 0.1f};
            break;
        case EGameAction::kBackStrafe:
            m_position += { 0.0f, 0.0f, -0.1f};
            break;
        case EGameAction::kLeftStrafe:
            m_position += { -0.1f, 0.0f, 0.0f};
            break;
        case EGameAction::kRightStrafe:
            m_position += { 0.1f, 0.0f, 0.0f};
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
        EDirection dType = direction(EDirectionType::kHorizontal, dFactor);
        camera()->moveCamera(dType, std::fabs(dFactor), 16.f);
    }

    if (fy != 0) {
        float dFactor = vDirectionFactor(fy, scale, false);
        EDirection dType = direction(EDirectionType::kVertical, dFactor);
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

EDirection Actor::direction(EDirectionType dType, float dFactor) const
{
    switch (dType) {
        case EDirectionType::kVertical:
            return (dFactor > 0) ? EDirection::kUp : EDirection::kDown;
        case EDirectionType::kHorizontal:
            return (dFactor < 0) ? EDirection::kLeft : EDirection::kRight;
        default:
            return EDirection::kNone;
    }

    return EDirection::kNone;
}
