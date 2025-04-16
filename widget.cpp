#include "widget.h"
#include "ui_widget.h"

#include <QSysInfo>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_frameTimer = new QTimer(this);
    connect(m_frameTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_frameTimer->start(1000 / 60.0f);
}

Widget::~Widget()
{
    delete ui;
    makeCurrent();
    delete m_program;
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    doneCurrent();
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();

    printf("GPU:\n");
    printf("    Vendor: %s\n", glGetString(GL_VENDOR));
    printf("    Device: %s\n", glGetString(GL_RENDERER));
    printf("    OpenGL: %s\n", glGetString(GL_VERSION));
    printf("    GLSL  : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    QSysInfo systemInfo;
    printf("Kernel:\n");
    printf("    Type: %s\n", systemInfo.kernelType().toStdString().c_str());
    printf("    Version: %s\n", systemInfo.kernelVersion().toStdString().c_str());

    printf("Product:\n");
    printf("    Type: %s\n", systemInfo.productType().toStdString().c_str());
    printf("    Name: %s\n", systemInfo.prettyProductName().toStdString().c_str());
    printf("    Version: %s\n", systemInfo.productVersion().toStdString().c_str());

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                       "#version 330 core\n"
                                       "layout(location = 0) in vec3 position;\n"
                                       "uniform mat4 mvp;\n"
                                       "void main() { gl_Position = mvp * vec4(position, 1.0); }");
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       "#version 330 core\n"
                                       "out vec4 fragColor;\n"
                                       "void main() { fragColor = vec4(0.8, 0.6, 0.2, 1.0); }");
    m_program->link();


    float vertices[] = {
        //x    //y   //z
        -0.5f, 0.5f, 0.0f,  // нижняя правая точка
        0.5f, 0.5f, 0.0f,   // верняя правая точка
        0.5f, -0.5f, 0.0f,  // верхняя левая точка
        -0.5f, -0.5f, 0.0f  // нижняя левая точка
    };


    GLuint indices[] = {
        0, 3, 1, 3, 2, 1  // квадрат
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    setup();
}

void Widget::resizeGL(int width, int height)
{
    glViewport(0,0, width, height);
}

void Widget::paintGL()
{
    m_actor->update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_program->bind();

    QMatrix4x4 projection;
    projection.perspective(45.0f, width() / float(height()), 0.1f, 100.0f);

    QMatrix4x4 view = m_actor->camera()->viewMatrix();
    view.translate(0,0,-2.0f);
    //qDebug() << view;
    //view.translate(0, 0, -3.0f);
    // 

    drawRoom(2,2, projection, view);

    m_program->release();
}


void Widget::drawRoom(int countHeight, int countWidht, QMatrix4x4 projection, QMatrix4x4 view)
{
    int countElement = (countHeight * 2) + (countWidht * 2);
    for (size_t i = 0; i <= countElement; i++)
    {
        QMatrix4x4 model;
        if(i < countHeight)
        {
            tempPos3DWalls += QVector3D(1.1f,0.0f,0.0f);
            elemPosWalls.emplace_back(tempPos3DWalls);
            model.translate(elemPosWalls.at(i).x(), elemPosWalls.at(i).y(), elemPosWalls.at(i).z());
        }
        else if(i > countHeight && i <= countWidht + countHeight)
        {
            tempPos3DWalls.setX(elemPosWalls.at(countHeight).x() - 0.5f);
            if(i == countHeight + 1)
            {
                tempPos3DWalls += QVector3D(0.0f,0.0f,0.5f);
            }
            else
            {
                tempPos3DWalls += QVector3D(0.0f,0.0f,1.1f);
            }
            elemPosWalls.emplace_back(tempPos3DWalls);
            model.translate(elemPosWalls.at(i).x(), elemPosWalls.at(i).y(), elemPosWalls.at(i).z());
            model.rotate(90.0f, 0.0f, 1.0f, 0.0f);
        }
        else if(i > countWidht + countHeight && i <= countWidht + countHeight + countHeight)
        {
            qDebug() << elemPosWalls.at(countWidht + countHeight).z();
            tempPos3DWalls.setZ(elemPosWalls.at(countWidht + countHeight).z() + 0.5f);
            if(i == countWidht + countHeight + 1)
            {
                qDebug() << elemPosWalls.at(countWidht + countHeight).x();
                tempPos3DWalls.setX(elemPosWalls.at(countWidht + countHeight).x() - 0.6f);
            }
            else
            {
                tempPos3DWalls += QVector3D(-1.1f,0.0f,0.0f);
            }
            elemPosWalls.emplace_back(tempPos3DWalls);
            model.translate(elemPosWalls.at(i).x(), elemPosWalls.at(i).y(), elemPosWalls.at(i).z());
        }
        else if(i > countWidht + countHeight + countHeight)
        {
            tempPos3DWalls.setX(elemPosWalls.at(countWidht + countHeight + countHeight).x() - 0.6f);
            if(i == countWidht + countHeight + countHeight + 1)
            {
                tempPos3DWalls.setZ(elemPosWalls.at(countWidht + countHeight).z());
            }
            else
            {
                tempPos3DWalls += QVector3D(0.0f,0.0f,-1.1f);
            }
            elemPosWalls.emplace_back(tempPos3DWalls);
            model.translate(elemPosWalls.at(i).x(), elemPosWalls.at(i).y(), elemPosWalls.at(i).z());
            model.rotate(90.0f, 0.0f, 1.0f,0.0f);
        }
        QMatrix4x4 mvp = projection * view * model;
        m_program->setUniformValue("mvp", mvp);
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    // test
    for (size_t i = 0; i <= countElement; i++)
    {   
        QMatrix4x4 model;
        if(i < countHeight)
        {
            tempPos3DFloors += QVector3D(1.1f,0.0f,0.0f);
            elemPosFloors.emplace_back(tempPos3DFloors);
            model.translate(elemPosFloors.at(i).x(), elemPosFloors.at(i).y(), elemPosFloors.at(i).z());
            model.rotate(90.0f, 1.0f,0.0f,0.0f);
        }
        QMatrix4x4 mvp = projection * view * model;
        m_program->setUniformValue("mvp", mvp);
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
}

void Widget::setup()
{
    /* Для отслеживания мыши без удержания LMB */
    setMouseTracking(true);

    m_actor = new Actor();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentMousePos = event->pos();

    int dx = currentMousePos.x() - m_lastMousePosition.x();
    int dy = currentMousePos.y() - m_lastMousePosition.y();

    m_actor->onRotate(dx, dy);
    m_lastMousePosition = currentMousePos;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) {
        m_actor->onAction(EGameAction::kForwardStrafe);
    }

    if (event->key() == Qt::Key_A) {
        m_actor->onAction(EGameAction::kRightStrafe);
    }

    if (event->key() == Qt::Key_S) {
        m_actor->onAction(EGameAction::kBackStrafe);
    }

    if (event->key() == Qt::Key_D) {
        m_actor->onAction(EGameAction::kLeftStrafe);
    }
}
