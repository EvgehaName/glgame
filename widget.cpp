#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
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
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f
    };


    GLuint indices[] = {
        0, 1, 2,  2, 3, 0,  // зад
        4, 5, 6,  6, 7, 4,  // перед
        0, 4, 7,  7, 3, 0,  // лево
        1, 5, 6,  6, 2, 1,  // право
        0, 1, 5,  5, 4, 0   // низ
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
    m_actor->camera()->update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_program->bind();

    QMatrix4x4 projection;
    projection.perspective(45.0f, width() / float(height()), 0.1f, 100.0f);

    QMatrix4x4 view = m_actor->camera()->viewMatrix();
    view.translate(0,0,0.0f);
    //qDebug() << view;
    //view.translate(0, 0, -3.0f);

    QMatrix4x4 model;
    model.rotate(0.0f, 0.0f, 0.0f);

    QMatrix4x4 mvp = projection * view * model;
    m_program->setUniformValue("mvp", mvp);


    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

    m_program->release();

    update();
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

    m_actor->onMouseMove(dx, dy);
    m_lastMousePosition = currentMousePos;
}
