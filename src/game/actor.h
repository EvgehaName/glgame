#ifndef ACTOR_H
#define ACTOR_H

#include "fpscamera.h"

namespace collision {
    class collision_object;
}

class Level;

class Actor
{
public:
    enum CamStyle{
        camFirst = 0,
        camFree,
        camMaxElem
    };
    Actor(Level* level);

    void onAction(const MovementState& state, float deltaTime);
    void onRotate(int dx, int dy);
    void update();
    void changeCamera(CameraBase& activeCamera);

    CameraBase * camera();

    inline const QVector3D& position() const { return m_position;  }
    inline collision::collision_object* collision() const { return m_collision; }

    float hDirectionFactor(float x, float scaleX, bool invertX) const;
    float vDirectionFactor(float y, float scaleY, bool invertY) const;
    EInputScreenDirection toScreenDirection(EInputAxis axis, float value) const;

private:
    float m_baseFov = 67.5f;
    float m_mouseSens = 0.12f;
    float m_lookFactor = 1.0f;
    float m_mouseSensScale = 1.0f;
    Level* m_level;
    collision::collision_object* m_collision;
    QVector3D m_position;
    FPSCamera m_camera;

    CameraBase* m_cameras[camMaxElem];
    int activeCam = 0;
};

#endif // ACTOR_H
