#pragma once

#include "actor.h"
#include "plain.h"
#include "camerabase.h"
#include "render/debugrenderer.h"
#include "dynamics/collision/collision_base.h"

constexpr const char* UNTITLED_PROJECT_NAME = "Untitled Project";

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
    void add_collision_object(Collision* collision_ptr);
    QMatrix4x4 getViewProjection() const;

    bool load(const QString& filepath);
    void load(const QJsonObject& config);
    void onLevelLoaded();

    /* EDITOR MODE ONLY */
    inline void setTreeItem(QTreeWidgetItem* item) { m_rootItem = item; }
    inline QTreeWidgetItem* treeItem() const { return m_rootItem; }

private:
	std::vector<GameObject*> m_objects;
    std::vector<Collision*> m_collisions;

	std::unique_ptr<Actor> m_actor;
	std::vector<std::unique_ptr<Plain>> m_floor;
	std::vector<std::unique_ptr<Plain>> m_walls;

    DebugRenderer * m_dbgRender;

	QMatrix4x4 m_projection;
	QOpenGLTexture* m_wallTexture;
	QOpenGLTexture* m_floorTexture;
	QOpenGLShaderProgram* m_shader;

    /* EDITOR MODE ONLY */
    QTreeWidgetItem* m_rootItem{nullptr};
};
