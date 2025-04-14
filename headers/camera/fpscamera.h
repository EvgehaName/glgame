#ifndef FPSCAMERA_H
#define FPSCAMERA_H
#include "camerabase.h"

class FPSCamera : public CameraBase
{
public:
    FPSCamera();

    void update() override;
    QMatrix4x4 viewMatrix() override;
    void moveCamera(EDirection direction, float dFactor, float deltaTime) override;
};

#endif // FPSCAMERA_H
