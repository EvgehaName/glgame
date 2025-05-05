////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H
#include <ode/ode.h>

class PhysicsWorld final
{
public:
    struct Config {
        float gravity    = -2.f * 9.81f;      // Гравитация
        int iterations   = 20;            // Количество итераций решателя
        float cfm        = 1e-5f;         // Constraint Force Mixing
        float erp        = 0.2f;          // Error Reduction Parameter
        int maxContacts  = 8;             // Количество контактов
        float fixedStep  = 0.016f;        // Фиксированный шаг 16 мс (60 FPS)
    };

    static PhysicsWorld& getInstance(const Config& config = Config()); // singleton meyers
    void tick(float dt);

    inline dWorldID getWorld() const { return m_world; }
    inline dSpaceID getSpace() const { return m_space; }

    PhysicsWorld(const PhysicsWorld&) = delete; // non construction-copyable
    PhysicsWorld& operator=(const PhysicsWorld&) = delete; // non copyable

private:
    Config m_config;
    dWorldID m_world;
    dSpaceID m_space;
    dJointGroupID m_contactGroup;

    explicit PhysicsWorld(const Config &config);
    ~PhysicsWorld();

    void setupWorld();
    void cleanupWorld();
    static void nearCallback(void *data, dGeomID o1, dGeomID o2);
};

#endif // PHYSICS_WORLD_H
