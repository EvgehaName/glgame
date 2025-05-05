////////////////////////////////////////////////////////////////////////////
//	Created 	: 01.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "application.h"
#include "ui_editor.h"

constexpr int MIN_WIDTH = 800;
constexpr int MIN_HEIGHT = 600;
constexpr dReal PHYS_WORLD_GRAVITY = -9.81;

#include <qmessagebox.h>
#include "dynamics/physics_world.h"

Application::Application(bool editorMode)
    : m_game(nullptr)
    , m_timer(nullptr)
    , m_audioCtx(nullptr)
    , m_levelLoader(nullptr)
    , m_dt(0.0f)
    , m_activated(false)
    , m_editorMode(editorMode)
    , m_levelLoaded(false)
    , m_glContextValid(false)
    , m_editor_ui(nullptr)
{
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    initialize();

#ifdef QT_DEBUG
    if (m_editorMode)
    {
        m_editor_ui = new Ui::Editor;
        m_editor_ui->setupUi(this);

        m_game = new Game(this, m_editor_ui->openGLWidget->parentWidget());
        m_game->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        m_editor_ui->sceneVerticalLayout->insertWidget(1, m_game);
        m_editor_ui->sceneVerticalLayout->removeWidget(m_editor_ui->openGLWidget);

        delete m_editor_ui->openGLWidget;
        m_editor_ui->openGLWidget = m_game;
    } else {
        setup();
    }
#endif // QT_DEBUG

#ifndef QT_DEBUG
    setup();
#endif // QT_DEBUG

    m_timer = new QTimer(this);
    m_timer->setInterval(1000 / 60.0f);
    connect(m_timer, &QTimer::timeout, this, &Application::tick);
    m_dt = 16;
}

Application::~Application()
{
    deinitialize();

    if (m_game) {
        delete m_game;
    }

    if (m_editor_ui) {
        delete m_editor_ui;
    }
}

void Application::loadLevel(const QString &filepath)
{    
    if (!m_glContextValid)
    {
        QMessageBox msgBox;
        msgBox.critical(this, "Failed to load level", "No valid OpenGL context");
    }
    else
    {
        unloadLevel();

        Level * level = new Level;
        if (m_levelLoader->load(filepath, level))
        {
            m_game->setLevel(level);
            level->onLevelLoaded();
            m_timer->start();
            m_levelLoaded = true;
        }
        else
        {
            QMessageBox msgBox;
            msgBox.critical(this, "Failed to load level", m_levelLoader->getLog());
        }
    }
}

void Application::unloadLevel()
{
    if (m_game->level())
    {
        m_timer->stop();
        delete m_game->level();
        m_game->setLevel(nullptr);
    }

    m_levelLoaded = false;
}

void Application::tick()
{
    if (m_levelLoaded) {
        PhysicsWorld::getInstance().tick(m_dt);
    }

    if (m_activated) {
        m_game->tick(m_dt);
    }
}

void Application::onActivate()
{

}

void Application::onDeactivate()
{

}

void Application::onGLContextCreated()
{
    m_glContextValid = true;
    loadLevel(":/data/room.json");
}

void Application::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if (m_activated) {
            onDeactivate();
            m_activated = false;
        } else {
            onActivate();
            m_activated = true;
        }
    }

    QMainWindow::changeEvent(event);
}

void Application::setup()
{
    setFixedSize(MIN_WIDTH, MIN_HEIGHT);
    m_game = new Game(this, this);
    m_game->resize(width(), height());
}

void Application::initialize()
{
    PhysicsWorld::getInstance();

    m_levelLoaded = new LevelLoader();
    m_audioCtx = new AudioContext();
}

void Application::deinitialize()
{
    unloadLevel();

    if (m_levelLoader) {
        delete m_levelLoader;
        m_levelLoader = nullptr;
    }

    if (m_audioCtx) {
        delete m_audioCtx;
        m_audioCtx = nullptr;
    }
}

#include "editor/options_window.h".h"
void Application::on_options_triggered()
{
    OptionsWindow * pOptionsModal = new OptionsWindow(this);
    pOptionsModal->setWindowModality(Qt::WindowModality::WindowModal);
    pOptionsModal->show();

}

