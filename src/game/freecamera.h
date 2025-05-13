#ifndef FREECAMERA_H
#define FREECAMERA_H
#include "camerabase.h"

class FREECamera : public CameraBase
{
public:
    FREECamera();

    QMatrix4x4 viewMatrix() override;
    virtual void update(const QVector3D& position) override;
    void moveCamera(EInputScreenDirection direction, float dFactor, float deltaTime) override;
};

#endif // FREECAMERA_H
