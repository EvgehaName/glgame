#pragma once
#include "gameobject.h"

class Plain : public GameObject, public RenderGeometry
{
public:
	Plain();
	Plain(const QJsonArray& pos, const QJsonArray& rot, const QJsonArray& scl);
	void what() override {};
};
