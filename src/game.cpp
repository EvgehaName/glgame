#include "game.h"

#include <QSysInfo>
#include <qapplication.h>

#include "custom_events.h"
#include "console_commands.h"
#include "core/engine.h"
#include "application.h"

Game::Game(Application * application, QWidget *parent)
    : QOpenGLWidget(parent)
    , m_application(application)
    , m_level(nullptr)
{
    // https://stackoverflow.com/questions/35245917/cant-capture-qkeyevent-in-qopenglwidget
    setFocusPolicy(Qt::StrongFocus);

    m_consoleWidget = new GameConsole(this);
    m_consoleWidget->hide();

    m_hud = new Hud(this);
}

Game::~Game()
{
    cleanup();
}

void Game::initializeGL()
{
    initializeOpenGLFunctions();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &Game::cleanup);

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

    m_application->onGLContextCreated();
}

void Game::resizeGL(int width, int height)
{
    glViewport(0,0, width, height);

    m_hud->onResize(width, height);
    m_level->onFramebufferResize(width, height);
    m_consoleWidget->setGeometry(0, 0, width, height >> 1);
}

void Game::paintGL()
{
    QPainter painter(this);

    /* Notify QPainter that we are going to start drawing with OpenGL */
    painter.beginNativePainting();
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Some sensible OpenGL settings */
        glFrontFace(GL_CW);
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        m_level->render();
#ifdef QT_DEBUG
        //m_dbgRender->render(m_level->getViewProjection());
#endif // QT_DEBUG
    }

    /* QPainter expects the following settings */
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);

    /* When we have finished with OpenGL we have to call the following function to notify QPainter */
    painter.endNativePainting();

    /* Over draw using QPainter */
#ifdef QT_DEBUG
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16));
    QString posText = QString("X:%1Y:%2Z:%3").arg(  QString::number(m_level->actor()->camera()->position().x()),
                                                    QString::number(m_level->actor()->camera()->position().y()),
                                                    QString::number(m_level->actor()->camera()->position().z()));
    painter.drawText(10,QWidget::height() - 10,"pos: " + posText);
#endif // QT_DEBUG

    /* End draw QPainter */
    painter.end();
}

void Game::tick(float dt)
{
    if (!hasFocus() && !m_consoleWidget->consoleHasFocus()) {
        return;
    }

    mouseMove();
    m_level->update(m_movementState);

    /* OpenGL */
    update();
}

void Game::setup()
{
    /* Для отслеживания мыши без удержания LMB */
    setMouseTracking(true);

    /* Скрывает курсор (если поддерживается) */
    setCursor(Qt::BlankCursor);

    m_consoleWidget->registerCommand("quit", [&](const CCommand& cmd) {
        QApplication::sendEvent(this, new QEvent(static_cast<QEvent::Type>(custom_events::Quit)));
        QApplication::quit();
    });

    m_consoleWidget->registerCommand("r_fullscreen", [&](const CCommand& cmd) {
        if (cmd.argv(1) == "1") {
            setWindowState(Qt::WindowFullScreen);
        } else if (cmd.argv(1) == "0") {
            setWindowState(Qt::WindowNoState);
        }
    });

    m_consoleWidget->registerCommand("soundstop", [&](const CCommand& cmd) {
        audioSound->stop();
    });

    m_consoleWidget->registerCommand("soundplay", [&](const CCommand& cmd) {
        audioSound->play(false);
    });

    /* Initialize */
    Engine::get();

   //m_level = new Level();

    audioLoader = new AudioLoader("D:\\Projects\\glgame\\src\\sound\\step.ogg");
    audioSound = new AudioSound(audioLoader->getPCM(), audioLoader->getFormat(), audioLoader->getSampleRate());

    // m_dbgRender = new DebugRenderer();
    // m_dbgRender->drawAllGeom();

    qDebug() << "setup success";
}

void Game::mouseMove()
{
    /* Позиция курсора и центр вьюпорта в экранных координатах */
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

void Game::cleanup()
{
    makeCurrent();
    {
        if (m_openglLogger) {
            delete m_openglLogger;
        }

        delete m_level;
    }
    doneCurrent();
    disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &Game::cleanup);
}

void Game::closeEvent(QCloseEvent *event)
{

}

void Game::keyPressEvent(QKeyEvent *event)
{
    int key = event->nativeVirtualKey();
    qDebug() << key;

    /* Скрытие или показ окна консоли */
    if (key == Qt::Key_Agrave) {
        if (m_consoleWidget->isHidden()) {
            m_consoleWidget->show();
        } else {
            m_consoleWidget->hide();
        }
    }

    if (key == Qt::Key_W && !event->isAutoRepeat()) {
       m_movementState.m_forward = true;
    }

    if (key == Qt::Key_A && !event->isAutoRepeat()) {
        m_movementState.m_right = true;
    }

    if (key == Qt::Key_S && !event->isAutoRepeat()) {
        m_movementState.m_back = true;
    }

    if (key == Qt::Key_D && !event->isAutoRepeat()) {
        m_movementState.m_left = true;
    }

    if (key == Qt::Key_Shift) {
        m_movementState.m_running = true;
    }

    if (m_movementState.m_forward == true || m_movementState.m_back  == true 
        || m_movementState.m_left == true || m_movementState.m_right == true)
    {
        if(!audioSound->isPlaying()) {
            audioSound->play(true);
        }
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->nativeVirtualKey();

    if (key == Qt::Key_W && !event->isAutoRepeat()) {
        m_movementState.m_forward = false;
    }

    if (key == Qt::Key_A && !event->isAutoRepeat()) {
        m_movementState.m_right = false;
    }

    if (key == Qt::Key_S && !event->isAutoRepeat()) {
        m_movementState.m_back = false;
    }

    if (key == Qt::Key_D && !event->isAutoRepeat()) {
        m_movementState.m_left = false;
    }

    if (key == Qt::Key_Shift) {
        m_movementState.m_running = false;
    }

    if (m_movementState.m_forward == false && m_movementState.m_back  == false 
        && m_movementState.m_left == false && m_movementState.m_right == false)
    {
        if(audioSound->isPlaying()) {
            audioSound->stop();
        }
    }
}

void Game::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);

    /* Если окно потеряло фокус, и это не консоль или пользователь, возвращаем */
    if (!m_consoleWidget->consoleHasFocus() && event->reason() != Qt::ActiveWindowFocusReason) {
        setFocus();
    }
}
