#ifndef FPSCAMERA_H
#define FPSCAMERA_H
#include "camerabase.h"

class FreeCamera : public CameraBase
{
public:
    FreeCamera();

    QMatrix4x4 viewMatrix() override;
    virtual void update(const QVector3D& position) override;
    void moveCamera(EInputScreenDirection direction, float dFactor, float deltaTime) override;
};

#endif // FREECAMERA_H
