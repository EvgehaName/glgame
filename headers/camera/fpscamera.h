#ifndef FPSCAMERA_H
#define FPSCAMERA_H
#include "camerabase.h"

class FPSCamera : public CameraBase
{
public:
    FPSCamera();

    QMatrix4x4 viewMatrix() override;
    virtual void update(const QVector3D& cameraPosition) override;
    void moveCamera(EDirection direction, float dFactor, float deltaTime) override;
};

#endif // FPSCAMERA_H
