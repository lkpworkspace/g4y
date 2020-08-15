#include "GPhyWorld.h"
#include "GCommon.h"
#include "GCollider.h"
#include "GRigibody.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"

void GPhyWorld::Init()
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

bool GPhyWorld::RayTest(glm::vec3 from, glm::vec3 to, GRayHit& hit)
{
    btVector3 f(from.x, from.y, from.z);
    btVector3 t(to.x, to.y, to.z);
    btCollisionWorld::ClosestRayResultCallback ray_cb(f, t);
    ray_cb.m_flags |= btTriangleRaycastCallback::kF_UseGjkConvexCastRaytest;
    m_dynamics_world->rayTest(f, t, ray_cb);
    if(ray_cb.hasHit()){
        btVector3 pick_pos = ray_cb.m_hitPointWorld;
        hit.pick_pos = glm::vec3(pick_pos[0], pick_pos[1], pick_pos[2]);
        void* p = ray_cb.m_collisionObject->getUserPointer();
        auto com = static_cast<GCom*>(p);
        hit.obj = com->Obj();
        return true;
    }
    return false;
}

void GPhyWorld::UpdateDynamicsWorld()
{
    m_dynamics_world->stepSimulation (1.f /60.f, 10);

    m_all_col_objs.clear();
    for(int i =  m_dynamics_world->getNumCollisionObjects() - 1; i >= 0; --i){
        btCollisionObject* obj = m_dynamics_world->getCollisionObjectArray()[i];
        // btRigidBody* body = btRigidBody::upcast(obj) ;
        // btTransform trans;
        // if(body && body->getMotionState()){
        //     body->getMotionState()->getWorldTransform(trans);
        // }else{
        //     trans = obj->getWorldTransform();
        // }
        // btScalar x, y, z;
        // trans.getRotation().getEulerZYX(x, y, z);
        // printf("world pos object %d = %f, %f, %f; %f %f %f\n", 
        //     i, 
        //     float(trans.getOrigin().getX()), 
        //     float(trans.getOrigin().getY()), 
        //     float(trans.getOrigin().getZ()),
        //     x, y, z
        // );
        m_all_col_objs.push_back(obj);
    }

    int num_manifolds = m_dynamics_world->getDispatcher()->getNumManifolds();
    for(int i = 0; i < num_manifolds; ++i){
        btPersistentManifold* contact_manifold = m_dynamics_world->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contact_manifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contact_manifold->getBody1());
        void* pA = obA->getUserPointer();
        void* pB = obB->getUserPointer();
        auto comA = static_cast<GCom*>(pA);
        auto comB = static_cast<GCom*>(pB);

        int num_contacts = contact_manifold->getNumContacts();

        // printf("collision A: %s, B: %s, i: %d, num_point: %d\n", comA->ComName().c_str(), comB->ComName().c_str(), i, num_contacts);
        for(int j = 0; j < num_contacts; ++j){
            btManifoldPoint& pt = contact_manifold->getContactPoint(j);
            // printf("distance %f\n", pt.getDistance());
            if(pt.getDistance() <= 0.f){
                if(comA->TypeName() == typeid(GCollider).name()){
                    auto collider = static_cast<GCollider*>(comA);
                    collider->OnCollision(obB);
                }else{
                    auto rigibody = static_cast<GRigibody*>(comA);
                    rigibody->OnCollision(obB);
                }
                if(comB->TypeName() == typeid(GCollider).name()){
                    auto collider = static_cast<GCollider*>(comB);
                    collider->OnCollision(obA);
                }else{
                    auto rigibody = static_cast<GRigibody*>(comB);
                    rigibody->OnCollision(obA);
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

    // 调用所有碰撞对象的OnCollisionEnd函數
    for(const auto& o : m_all_col_objs){
        void* pobj = o->getUserPointer();
        auto com_obj = static_cast<GCom*>(pobj);
        if(com_obj->TypeName() == typeid(GCollider).name()){
            auto collider = static_cast<GCollider*>(com_obj);
            collider->OnCollisionEnd();
        }else if(com_obj->TypeName() == typeid(GRigibody).name()){
            auto rigibody = static_cast<GRigibody*>(com_obj);
            rigibody->OnCollisionEnd();
		}
		else {
			assert("collider");
		}
    }
}