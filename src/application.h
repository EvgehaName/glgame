////////////////////////////////////////////////////////////////////////////
//	Created 	: 01.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include <QEvent>

#include <ode/ode.h>

#include "game.h"
#include "core/audio_context.h"
#include "core/level_loader.h"

namespace Ui {
    class Editor;
}

class Application : public QMainWindow
{
    Q_OBJECT
public:
    Application(bool editorMode);
    ~Application();

    void loadLevel(const QString& filepath);
    void unloadLevel();

    inline bool isEditorMode() const { return m_editorMode; }

    void tick();
    void onActivate();
    void onDeactivate();
    void onGLContextCreated();

protected:
    void changeEvent(QEvent *event);

private slots:
    void on_options_triggered();

private:
    Game * m_game;
    QTimer * m_timer;
    AudioContext * m_audioCtx;
    LevelLoader * m_levelLoader;

    float m_dt;
    bool m_activated;
    bool m_editorMode;
    bool m_levelLoaded;
    bool m_glContextValid;
    Ui::Editor * m_editor_ui;

    void setup();
    void initialize();
    void deinitialize();
};

#endif // APPLICATION_H
