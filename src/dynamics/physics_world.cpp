////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "physics_world.h"
#include <vector>

PhysicsWorld &PhysicsWorld::getInstance(const Config &config)
{
    static PhysicsWorld instance(config);
    return instance;
}

void PhysicsWorld::tick(float dt)
{
    dSpaceCollide(m_space, this, &PhysicsWorld::nearCallback);
    dWorldStep(m_world, m_config.fixedStep);
    dJointGroupEmpty(m_contactGroup);
}

PhysicsWorld::PhysicsWorld(const Config &config)
    : m_config(config)
{
    dInitODE2(0);
    setupWorld();
}

PhysicsWorld::~PhysicsWorld()
{
    cleanupWorld();
    dCloseODE();
}

void PhysicsWorld::setupWorld()
{
    m_world = dWorldCreate();
    dWorldSetGravity(m_world, 0.0f, m_config.gravity, 0.0f);

    // Жесткость и демпфирование (ERP, CFM)
    dWorldSetERP(m_world, 0.8);
    dWorldSetCFM(m_world, 1e-5);

    m_space = dSimpleSpaceCreate(0);
    m_contactGroup = dJointGroupCreate(0);
}

void PhysicsWorld::cleanupWorld()
{
    if (m_contactGroup) {
        dJointGroupEmpty(m_contactGroup);
        dJointGroupDestroy(m_contactGroup);
    }

    if (m_space) {
        dSpaceDestroy(m_space);
    }

    if (m_world) {
        dWorldDestroy(m_world);
    }
}

void PhysicsWorld::nearCallback(void *data, dGeomID o1, dGeomID o2)
{
    PhysicsWorld * self = static_cast<PhysicsWorld*>(data);

    // Temporary index for each contact
    int i = 0;

    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    // Если два тела соединены суставом, ничего не делаем
    if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) return;

    // Maximum number of contacts to create between bodies (see ODE documentation)
    std::vector<dContact> contacts;
    contacts.resize(self->m_config.maxContacts);

    for (i = 0; i < self->m_config.maxContacts; i++)
    {
        contacts[i].surface.mode       = dContactMotion1;
        contacts[i].surface.mu         = dInfinity;
        contacts[i].surface.bounce     = 0.0f;
        contacts[i].surface.bounce_vel = 0.0f;
        contacts[i].surface.soft_erp   = 0.8;
        contacts[i].surface.soft_cfm   = 0.01f;
    }

    // Add collision joints
    if (int numc = dCollide(o1, o2, self->m_config.maxContacts, &contacts[0].geom, sizeof(dContact)))
    {
        for (i = 0; i < numc; i++)
        {
            dJointID c = dJointCreateContact(self->m_world, self->m_contactGroup, &contacts[i]);
            dJointAttach(c, b1, b2);
        }
    }
}
