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

	void onAxisMove(int dx, int dy);
	void onFramebufferResize(int width, int height);

private:
	Actor* m_actor;
	Plain* m_floor;
	Plain* m_walls[4];
	QMatrix4x4 m_modeles[4];
	QMatrix4x4 m_projection;
	QOpenGLTexture* m_wallTexture;
	QOpenGLTexture* m_floorTexture;
	QOpenGLShaderProgram* m_shader;
};