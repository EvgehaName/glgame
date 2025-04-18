#include "widget.h"
#include "ui_widget.h"

#include <QSysInfo>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_frameTimer = new QTimer(this);
    connect(m_frameTimer, SIGNAL(timeout()), this, SLOT(frameTick()));
    m_frameTimer->start(1000 / 60.0f);
}

Widget::~Widget()
{
    delete ui;
    makeCurrent();
    delete m_program;
    textureMap["wall"]->destroy();
    textureMap["floor"]->destroy();
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
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexShader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "fragmentShader.glsl");
    m_program->link();


    float vertices[] = {
        //x    //y   //z    // uv
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // нижняя правая точка
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // верняя правая точка
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // верхняя левая точка
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f // нижняя левая точка
    };


    GLuint indices[] = {
        0, 3, 1, 3, 2, 1  // квадрат
    };

    textureMap["wall"] = new QOpenGLTexture(QImage(":/textures/wall_basecolor.png").mirrored());
    textureMap["wall"]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureMap["wall"]->setMagnificationFilter(QOpenGLTexture::Linear);
    textureMap["wall"]->setWrapMode(QOpenGLTexture::Repeat);

    textureMap["floor"] = new QOpenGLTexture(QImage(":/textures/floor_basecolor.png").mirrored());
    textureMap["floor"]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureMap["floor"]->setMagnificationFilter(QOpenGLTexture::Linear);
    textureMap["floor"]->setWrapMode(QOpenGLTexture::Repeat);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    drawRoom(2,4,projection, view);

    m_program->release();
}


void Widget::drawRoom(int countHeight, int countWidht, QMatrix4x4 projection, QMatrix4x4 view)
{
    int countElement = (countHeight * 2) + (countWidht * 2);

    textureMap["wall"]->bind(0);
    m_program->setUniformValue("textureSampler", 0);

    // draw walls
    for (size_t i = 0; i <= countElement; i++)
    {
        QMatrix4x4 model;
        if(i < countHeight)
        {
            tempPos3DWalls += QVector3D(1.0f,0.0f,0.0f);
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
                tempPos3DWalls += QVector3D(0.0f,0.0f,1.0f);
            }
            elemPosWalls.emplace_back(tempPos3DWalls);
            model.translate(elemPosWalls.at(i).x(), elemPosWalls.at(i).y(), elemPosWalls.at(i).z());
            model.rotate(90.0f, 0.0f, 1.0f, 0.0f);
        }
        else if(i > countWidht + countHeight && i <= countWidht + countHeight + countHeight)
        {
            tempPos3DWalls.setZ(elemPosWalls.at(countWidht + countHeight).z() + 0.5f);
            if(i == countWidht + countHeight + 1)
            {
                tempPos3DWalls.setX(elemPosWalls.at(countWidht + countHeight).x() - 0.5f);
            }
            else
            {
                tempPos3DWalls += QVector3D(-1.0f,0.0f,0.0f);
            }
            elemPosWalls.emplace_back(tempPos3DWalls);
            model.translate(elemPosWalls.at(i).x(), elemPosWalls.at(i).y(), elemPosWalls.at(i).z());
        }
        else if(i > countWidht + countHeight + countHeight)
        {
            tempPos3DWalls.setX(elemPosWalls.at(countWidht + countHeight + countHeight).x() - 0.5f);
            if(i == countWidht + countHeight + countHeight + 1)
            {
                tempPos3DWalls.setZ(elemPosWalls.at(countWidht + countHeight).z());
            }
            else
            {
                tempPos3DWalls += QVector3D(0.0f,0.0f,-1.0f);
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

    textureMap["floor"]->bind(0);
    m_program->setUniformValue("textureSampler", 0);

    // draw floor
    for (size_t i = 0; i < countHeight; i++)
    {   
        for (size_t t = 0; t < countWidht; t++)
        {
            QMatrix4x4 model;
            model.translate(i * 1.0f, -0.5f, 0.5f + t * 1.0f);
            model.rotate(90.0f, 1.0f, 0.0f,0.0f);
            QMatrix4x4 mvp = projection * view * model;
            m_program->setUniformValue("mvp", mvp);
            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }        
    }
}

void Widget::setup()
{
    /* Для отслеживания мыши без удержания LMB */
    setMouseTracking(true);

    /* Скрывает курсор (если поддерживается) */
    setCursor(Qt::BlankCursor);

    m_actor = new Actor();
}

void Widget::mouseMove()
{
    /* Позиция курсора и центр окна в экранных координатах */
    QPoint currentPos = QCursor::pos();
    QPoint windowCenter = mapToGlobal(rect().center());

    /* Смещение относительно центра */
    int offsetx = currentPos.x() - windowCenter.x();
    int offsety = currentPos.y() - windowCenter.y();

    if (!offsetx && !offsety)
        return;

    m_actor->onRotate(offsetx, offsety);

    /* Центрируем курсор */
    QCursor::setPos(windowCenter);
}

/* Вызывается каждый кадр */
void Widget::frameTick()
{
    mouseMove();

    /* OpenGL */
    update();
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
