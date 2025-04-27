#pragma once

#include "actor.h"
#include "plain.h"
#include "camerabase.h"
#include "collision_object.h"

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
	Actor* actor();

	void addGameObject(GameObject* ptr);
	void add_collision_object(collision::collision_object* collision_ptr);
    QMatrix4x4 getViewProjection() const;

	bool check_collide(collision::collision_object* who_collision_ptr);

private:
	std::vector<GameObject*> m_objects;
	std::vector<collision::collision_object*> m_collisions;

	std::unique_ptr<Actor> m_actor;
	std::vector<std::unique_ptr<Plain>> m_floor;
	std::vector<std::unique_ptr<Plain>> m_walls;

	QMatrix4x4 m_projection;
	QOpenGLTexture* m_wallTexture;
	QOpenGLTexture* m_floorTexture;
	QOpenGLShaderProgram* m_shader;
};
