#pragma once

#include "actor.h"
#include "plain.h"
#include "camerabase.h"

class Level
{
public:
	Level();
	~Level();

	void init();
	void update(const MovementState& movement);
	void render();

	void createRoom(int size);
	void onAxisMove(int dx, int dy);
	void onFramebufferResize(int width, int height);

private:
	std::unique_ptr<Actor> m_actor;
	std::vector<std::unique_ptr<Plain>> m_floor;
	std::vector<std::unique_ptr<Plain>> m_walls;

	QMatrix4x4 m_projection;
	QOpenGLTexture* m_wallTexture;
	QOpenGLTexture* m_floorTexture;
	QOpenGLShaderProgram* m_shader;
};