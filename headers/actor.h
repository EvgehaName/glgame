#ifndef ACTOR_H
#define ACTOR_H

#include "camera/fpscamera.h"

class Actor
{
public:
    Actor();

    void onMouseMove(int dx, int dy);
    CameraBase * camera();

    float hDirectionFactor(float x, float scaleX, bool invertX) const;
    float vDirectionFactor(float y, float scaleY, bool invertY) const;
    EDirection direction(EDirectionType dType, float dFactor) const;

private:
    float m_baseFov = 67.5f;
    float m_mouseSens = 0.12f;
    float m_lookFactor = 1.0f;
    float m_mouseSensScale = 1.0f;
    CameraBase * m_camera;
};

#endif // ACTOR_H
