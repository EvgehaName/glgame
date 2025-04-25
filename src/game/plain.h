#pragma once
#include "gameobject.h"

class Plain : public GameObject, public RenderGeometry
{
public:
	Plain();
	void render() override;
};