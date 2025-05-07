////////////////////////////////////////////////////////////////////////////
//	Created 	: 01.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef ENGINE_H
#define ENGINE_H

#include <QMainWindow>
#include <QEvent>

#include <ode/ode.h>

#include "game.h"
#include "core/audio_context.h"

#include <QTreeWidget>

namespace Ui {
    class Editor;
}

class Engine : public QMainWindow
{
    Q_OBJECT
public:
    Engine(bool editorMode);
    ~Engine();

    void loadLevel(const QString& filepath);
    void unloadLevel();

    QTreeWidget * projectTree() const;
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

#endif // ENGINE_H
