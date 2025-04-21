#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QTimer>

#include "actor.h"
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
    QOpenGLShaderProgram *m_program;
    GLuint m_vbo = 0;
    GLuint m_vao = 0;
    float m_angle = 0.0f;

    Actor * m_actor;
    GameConsole * m_consoleWidget;
    MovementState m_movementState;

    QMap<std::string, QOpenGLTexture*> textureMap;
    QOpenGLTexture *texture;
    QOpenGLTexture *texture1;

    std::vector<QVector3D> elemPosWalls {QVector3D(0.0f,0.0f,0.0f)};
    QVector3D tempPos3DWalls;

    QVector3D direction {QVector3D(1.5f, 2.0f, 10.0f)};
    
    void drawRoom(int countHeight, int countWidht, QMatrix4x4 projection, QMatrix4x4 view);

    void setup();

    void mouseMove();
    Q_SLOT void frameTick();


protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};
#endif // WIDGET_H
