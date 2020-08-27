#ifndef __GPHYWORLD_H__
#define __GPHYWORLD_H__
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "GConfig.hpp"
class GObj;
class btCollisionObject;
class btRigidBody;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class G4Y_DLL GRayHit
{
public:
    glm::vec3           pick_pos;
    std::weak_ptr<GObj> obj;
};

class G4Y_DLL GPhyWorld final
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

	void AddRigiBody(btRigidBody* rb);
	void DelRigiBody(btRigidBody* rb);

    bool RayTest(glm::vec3 from, glm::vec3 to, GRayHit& hit);

    void UpdateDynamicsWorld();

	void PreSimulate();

	void PostSimulate();
// private:
    std::shared_ptr<btDefaultCollisionConfiguration>        m_collision_cfg;
    std::shared_ptr<btCollisionDispatcher>                  m_collision_dispatcher;
    std::shared_ptr<btBroadphaseInterface>                  m_broadphase;
    std::shared_ptr<btSequentialImpulseConstraintSolver>    m_solver;
    std::shared_ptr<btDiscreteDynamicsWorld>                m_dynamics_world;

    std::vector<btCollisionObject*>                         m_all_col_objs;
};

#endif