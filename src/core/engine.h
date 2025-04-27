#ifndef ENGINE_H
#define ENGINE_H

#include <ode/ode.h>

class Engine
{
public:
    Engine();
    ~Engine();

    static Engine& get();

    inline dWorldID world() const { return m_phWorld; }
    inline dSpaceID space() const { return m_phSpace; }

private:
    dWorldID m_phWorld;
    dSpaceID m_phSpace;
    dJointGroupID m_phJointGroup;
};

#endif // ENGINE_H
