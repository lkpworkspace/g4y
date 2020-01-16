#include "GMeshCollider.h"
#include "GObj.h"
#include "GTransform.h"

 void GMeshCollider::Awake()
 {
    m_transform = Obj()->Transform();
    m_phy_world = Obj()->PhyWorld();
    m_ground       = std::static_pointer_cast<btCollisionShape>(std::make_shared<btStaticPlaneShape>(btVector3(0,1,0),1));
    m_motion_state = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, m_motion_state.get(), m_ground.get(), btVector3(0,0,0));
    m_rigidbody = std::make_shared<btRigidBody>(groundRigidBodyCI);

    m_phy_world.lock()->AddRigiBody(m_rigidbody);
}

void GMeshCollider::Update()
{
    btTransform trans;
    btScalar rx, ry, rz;
    m_rigidbody->getMotionState()->getWorldTransform(trans);
    trans.getRotation().getEulerZYX(rz, ry, rx);
    m_transform.lock()->postion = glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
    m_transform.lock()->rotate  = glm::vec3(rx, ry, rz);
}

void GMeshCollider::Exit()
{
    m_phy_world.lock()->DelRigiBody(m_rigidbody);
}