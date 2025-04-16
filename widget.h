#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QTimer>

#include "actor.h"

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
    QPoint m_lastMousePosition;

    struct PropertiesElem
    {
        QVector3D position;
        float angle;
        QVector3D rotation;
        QVector3D scale;
    };

    std::vector<PropertiesElem> elemPosition {
        // Стены
        {QVector3D(0.0f,0.0f,0.0f), 0.0f, QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // -*
        {QVector3D(1.1f,0.0f,0.0f), 0.0f, QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // --*
        {QVector3D(2.2f,0.0f,0.0f), 0.0f, QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // ---*
        {QVector3D(2.7f,0.0f,0.6f), 90.0f, QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //    |*
        {QVector3D(2.7f,0.0f,1.7f), 90.0f, QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //    |*
        {QVector3D(2.7f,0.0f,2.8f), 90.0f, QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //    |*
        {QVector3D(2.7f,0.0f,3.9f), 90.0f, QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //    |*
        {QVector3D(2.2f,0.0f,4.5f), 0.0f, QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // -*
        {QVector3D(1.1f,0.0f,4.5f), 0.0f, QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // --*
        {QVector3D(0.0f,0.0f,4.5f), 0.0f, QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // ---*
        {QVector3D(-0.5f,0.0f,0.6f), 90.0f, QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // |*
        {QVector3D(-0.5f,0.0f,1.7f), 90.0f, QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // |*
        {QVector3D(-0.5f,0.0f,2.8f), 90.0f, QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // |*
        {QVector3D(-0.5f,0.0f,3.9f), 90.0f, QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // |*
        // Пол
        {QVector3D(0.0f,-0.5f,0.6f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // |*
        {QVector3D(0.0f,-0.5f,1.7f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // |*
        {QVector3D(0.0f,-0.5f,2.8f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // |*
        {QVector3D(0.0f,-0.5f,3.9f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, // |*
        {QVector3D(1.1f,-0.5f,0.6f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //   |*
        {QVector3D(1.1f,-0.5f,1.7f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //   |*
        {QVector3D(1.1f,-0.5f,2.8f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //   |*
        {QVector3D(1.1f,-0.5f,3.9f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //   |*
        {QVector3D(2.2f,-0.5f,0.6f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //       |*
        {QVector3D(2.2f,-0.5f,1.7f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //       |*
        {QVector3D(2.2f,-0.5f,2.8f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //       |*
        {QVector3D(2.2f,-0.5f,3.9f), 90.0f, QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f)}, //       |*
    };

    void setup();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // WIDGET_H
