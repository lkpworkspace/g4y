#ifndef __GPHYWORLD_H__
#define __GPHYWORLD_H__
#include <string>
#include <memory>
#include <vector>
#include <btBulletDynamicsCommon.h>
#include "GCommon.h"

class GRayHit
{
public:
    glm::vec3           pick_pos;
    std::weak_ptr<GObj> obj;
};

class GPhyWorld
{
public:
    void Init();

    template<typename T>
    void AddCollisionObj(std::shared_ptr<T> co){
        auto o = std::dynamic_pointer_cast<btCollisionObject>(co);
        if(o){
            m_dynamics_world->addCollisionObject(o.get());
        }
    }

    template<typename T>
    void DelCollisionObj(std::shared_ptr<T> co){
        auto o = std::dynamic_pointer_cast<btCollisionObject>(co);
        if(o){
            m_dynamics_world->removeCollisionObject(o.get());
        }
    }

    template<typename T>
    void AddRigiBody(std::shared_ptr<T> rb){
        auto o = std::dynamic_pointer_cast<btRigidBody>(rb);
        if(o){
            m_dynamics_world->addRigidBody(o.get());
        }
    }

    template<typename T>
    void DelRigiBody(std::shared_ptr<T> rb){
        auto o = std::dynamic_pointer_cast<btRigidBody>(rb);
        if(o){
            m_dynamics_world->removeRigidBody(o.get());
        }
    }

    bool RayTest(glm::vec3 from, glm::vec3 to, GRayHit& hit);

    void UpdateDynamicsWorld();

// private:
    std::shared_ptr<btDefaultCollisionConfiguration>        m_collision_cfg;
    std::shared_ptr<btCollisionDispatcher>                  m_collision_dispatcher;
    std::shared_ptr<btBroadphaseInterface>                  m_broadphase;
    std::shared_ptr<btSequentialImpulseConstraintSolver>    m_solver;
    std::shared_ptr<btDiscreteDynamicsWorld>                m_dynamics_world;

    std::vector<btCollisionObject*>                         m_all_col_objs;
};

#endif