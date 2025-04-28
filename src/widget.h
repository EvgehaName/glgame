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
#include "audio/audio_context.h"
#include "audio/audio_loader.h"
#include "audio/audio_sound.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

// TODO: Engine (Facade class API)

class Widget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    Ui::Widget *ui;
    QTimer * m_frameTimer;
    QOpenGLDebugLogger* m_openglLogger{ nullptr };

    Level* m_level;
    Hud * m_hud;
    GameConsole * m_consoleWidget;
    MovementState m_movementState;

    AudioContext audioContext;
    AudioLoader* audioLoader;
    AudioSound* audioSound;
    DebugRenderer * m_dbgRender;

    void setup();
    void mouseMove();

    Q_SLOT void cleanup();
    Q_SLOT void frameTick();

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
