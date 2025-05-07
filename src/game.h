#ifndef WIDGET_H
#define WIDGET_H

#include "game/level.h"

#include <qopengldebug.h>
#include <qopenglwidget.h>
#include <qopenglfunctions_4_3_core.h>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QTimer>
#include <QPainter>

#include "hud.h"
#include "game_console.h"
#include "render/debugrenderer.h"

#include "core/audio_sound.h"
#include "dynamics/physics_world.h"
#include "core/audio_loader.h"

class Engine;

class Game : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    Game(Engine * application, QWidget *parent = nullptr);
    ~Game();

    inline void setLevel(Level* level) { m_level = level; }
    inline Level* level() const { return m_level; }
    void tick(float dt);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    Engine * m_application;
    QOpenGLDebugLogger* m_openglLogger{ nullptr };
    Level* m_level;

    Hud * m_hud;
    GameConsole * m_consoleWidget;
    MovementState m_movementState;

    AudioLoader* audioLoader;
    AudioSound* audioSound;

    void setup();
    void mouseMove();

    Q_SLOT void cleanup();

    /* PHYSICS SECTION START */
    dWorldID world;
    void initPhysics();
    /* PHYSICS SECTION END   */

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent* event) override;
};
#endif // WIDGET_H
