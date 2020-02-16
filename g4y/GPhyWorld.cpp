#include "GPhyWorld.h"
#include "GCommon.h"
#include "GCollider.h"
#include "GRigibody.h"

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

    int num_manifolds = m_dynamics_world->getDispatcher()->getNumManifolds();
    for(int i = 0; i < num_manifolds; ++i){
        btPersistentManifold* contact_manifold = m_dynamics_world->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contact_manifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contact_manifold->getBody1());

        int num_contacts = contact_manifold->getNumContacts();
        for(int j = 0; j < num_contacts; ++j){
            btManifoldPoint& pt = contact_manifold->getContactPoint(j);
            printf("distance %f\n", pt.getDistance());
            if(pt.getDistance() < 0.f){
                void* pA = obA->getUserPointer();
                void* pB = obB->getUserPointer();
                auto comA = static_cast<GCom*>(pA);
                auto comB = static_cast<GCom*>(pB);
                if(comA->ComName() == "GCollider"){
                    auto collider = static_cast<GCollider*>(comA);
                    collider->OnCollision();
                }else{
                    auto rigibody = static_cast<GRigibody*>(comA);
                    rigibody->OnCollision();
                }
                if(comB->ComName() == "GCollider"){
                    auto collider = static_cast<GCollider*>(comB);
                    collider->OnCollision();
                }else{
                    auto rigibody = static_cast<GRigibody*>(comB);
                    rigibody->OnCollision();
                }

                // const btVector3& ptA = pt.getPositionWorldOnA();
                // const btVector3& ptB = pt.getPositionWorldOnB();
                // printf("world col objectA %d = %f, %f, %f \nworld col objectB %d = %f, %f, %f \n", 
                //     i, 
                //     float(ptA.getX()), 
                //     float(ptA.getY()), 
                //     float(ptA.getZ()),
                //     i, 
                //     float(ptB.getX()), 
                //     float(ptB.getY()), 
                //     float(ptB.getZ())
                // );
            }
        }
    }
    
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