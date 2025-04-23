#pragma once
#include "gameobject.h"

class Cube : public GameObject, public RenderGeometry
{
public:
	Cube();

	void render() override;
	void load(const void* vData, const void* iData, int vSize, int iSize) override;
};