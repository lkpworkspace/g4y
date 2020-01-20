#include "GPhyWorld.h"

void GPhyWorld::InitPhysics()
{
    m_collision_cfg             = std::make_shared<btDefaultCollisionConfiguration>();
    m_collision_dispatcher      = std::make_shared<btCollisionDispatcher>(m_collision_cfg.get());
    m_broadphase                = std::static_pointer_cast<btBroadphaseInterface>(std::make_shared<btDbvtBroadphase>());
    m_solver                    = std::make_shared<btSequentialImpulseConstraintSolver>();
    m_dynamics_world            = std::make_shared<btDiscreteDynamicsWorld>(
        m_collision_dispatcher.get(),
        m_broadphase.get(),
        m_solver.get(),
        m_collision_cfg.get()
    );

    m_dynamics_world->setGravity(btVector3(0, -10, 0));
}

void GPhyWorld::UpdateDynamicsWorld()
{
    m_dynamics_world->stepSimulation (1.f /60.f, 10) ;
    for(int i =  m_dynamics_world->getNumCollisionObjects() - 1; i >= 0; --i){
        btCollisionObject* obj = m_dynamics_world->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj) ;
        btTransform trans;
        if(body && body->getMotionState()){
            body->getMotionState()->getWorldTransform(trans);
        }else{
            trans = obj->getWorldTransform();
        }
        // printf("world pos object %d = %f, %f, %f \n", 
        //     i, 
        //     float(trans.getOrigin().getX()), 
        //     float(trans.getOrigin().getY()), 
        //     float(trans.getOrigin().getZ())
        // );
    }
}