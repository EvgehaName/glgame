#include "widget.h"
#include "ui_widget.h"

#include <QSysInfo>
#include "console_commands.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // https://stackoverflow.com/questions/35245917/cant-capture-qkeyevent-in-qopenglwidget
    setFocusPolicy(Qt::StrongFocus);

    m_consoleWidget = new GameConsole(this);
    m_consoleWidget->hide();

    m_hud = new Hud(this);

    m_frameTimer = new QTimer(this);
    connect(m_frameTimer, SIGNAL(timeout()), this, SLOT(frameTick()));
    m_frameTimer->start(1000 / 60.0f);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &Widget::cleanup);

    printf("GPU:\n");
    printf("    Vendor: %s\n", glGetString(GL_VENDOR));
    printf("    Device: %s\n", glGetString(GL_RENDERER));
    printf("    OpenGL: %s\n", glGetString(GL_VERSION));
    printf("    GLSL  : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    QSysInfo systemInfo;
    printf("Kernel:\n");
    printf("    Type: %s\n", systemInfo.kernelType().toStdString().c_str());
    printf("    Version: %s\n", systemInfo.kernelVersion().toStdString().c_str());

    printf("Product:\n");
    printf("    Type: %s\n", systemInfo.productType().toStdString().c_str());
    printf("    Name: %s\n", systemInfo.prettyProductName().toStdString().c_str());
    printf("    Version: %s\n", systemInfo.productVersion().toStdString().c_str());
    
#ifdef QT_DEBUG
    m_openglLogger = new QOpenGLDebugLogger(this);
    QObject::connect(m_openglLogger, &QOpenGLDebugLogger::messageLogged, this, 
        [](QOpenGLDebugMessage message) {
            if (message.severity() != QOpenGLDebugMessage::NotificationSeverity) {
                qDebug() << message;
            }
    }, Qt::DirectConnection);

    Q_ASSERT(m_openglLogger->initialize());
    m_openglLogger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
    m_openglLogger->enableMessages();
#endif // QT_DEBUG

    setup();
}

void Widget::resizeGL(int width, int height)
{
    glViewport(0,0, width, height);

    m_hud->onResize(width, height);
    m_level->onFramebufferResize(width, height);
    m_consoleWidget->setGeometry(0, 0, width, height >> 1);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_level->render();
}

void Widget::setup()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    /* Для отслеживания мыши без удержания LMB */
    setMouseTracking(true);

    /* Скрывает курсор (если поддерживается) */
    setCursor(Qt::BlankCursor);

    m_consoleWidget->registerCommand("quit", [](const CCommand& cmd) {
        QApplication::quit();
    });

    m_consoleWidget->registerCommand("r_fullscreen", [&](const CCommand& cmd) {
        if (cmd.argv(1) == "1") {
            setWindowState(Qt::WindowFullScreen);
        } else if (cmd.argv(1) == "0") {
            setWindowState(Qt::WindowNoState);
        }
    });

    m_level = new Level();
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

    m_level->onAxisMove(offsetx, offsety);

    /* Центрируем курсор */
    QCursor::setPos(windowCenter);
}

Q_SLOT void Widget::cleanup()
{
    makeCurrent();
    {
        delete m_openglLogger;
        m_openglLogger = nullptr;

        delete m_level;
        m_level = nullptr;

        qDebug() << Q_FUNC_INFO << "OpenGL context destroyed";
    }
    doneCurrent();
    disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &Widget::cleanup);
}

/* Вызывается каждый кадр */
void Widget::frameTick()
{
    if (!hasFocus() && !m_consoleWidget->consoleHasFocus()) {
        return;
    }

    mouseMove();
    m_level->update(m_movementState);

    /* OpenGL */
    update();
}

void Widget::closeEvent(QCloseEvent *event)
{

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    /* Скрытие или показ окна консоли */
    if (event->key() == Qt::Key_QuoteLeft) {
        if (m_consoleWidget->isHidden()) {
            m_consoleWidget->show();
        } else {
            m_consoleWidget->hide();
        }
    }


    if (event->key() == Qt::Key_W) {
       m_movementState.m_forward = true;
    }

    if (event->key() == Qt::Key_A) {
        m_movementState.m_right = true;
    }

    if (event->key() == Qt::Key_S) {
        m_movementState.m_back = true;
    }

    if (event->key() == Qt::Key_D) {
        m_movementState.m_left = true;
    }
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) {
       m_movementState.m_forward = false;
    }

    if (event->key() == Qt::Key_A) {
        m_movementState.m_right = false;
    }

    if (event->key() == Qt::Key_S) {
        m_movementState.m_back = false;
    }

    if (event->key() == Qt::Key_D) {
        m_movementState.m_left = false;
    }
}

void Widget::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);

    /* Если окно потеряло фокус, и это не консоль или пользователь, возвращаем */
    if (!m_consoleWidget->consoleHasFocus() && event->reason() != Qt::ActiveWindowFocusReason) {
        setFocus();
    }
}
