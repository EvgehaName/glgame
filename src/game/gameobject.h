#pragma once
#include "../render/geom.h"

class GameObject
{
public:    
    GameObject() = default;

protected:
    QMatrix4x4 modelMatrix;
};