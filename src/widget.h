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

#include "hud.h"
#include "game_console.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

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

    void setup();
    void mouseMove();

    Q_SLOT void cleanup();
    Q_SLOT void frameTick();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent* event) override;
};
#endif // WIDGET_H
