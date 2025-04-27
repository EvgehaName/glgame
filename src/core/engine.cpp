#include "engine.h"

const dReal gravity = -9.81;

Engine::Engine()
{
    dInitODE();

    m_phWorld = dWorldCreate();
    dWorldSetGravity(m_phWorld, 0, 0, gravity);

    m_phSpace = dSimpleSpaceCreate(0);
    m_phJointGroup = dJointGroupCreate(0);
}

Engine::~Engine()
{
    dJointGroupDestroy(m_phJointGroup);
    dSpaceDestroy(m_phSpace);
    dWorldDestroy(m_phWorld);
}

Engine &Engine::get()
{
    static Engine engine;
    return engine;
}
