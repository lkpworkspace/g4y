#include "GMeshCollider.h"
#include "GObj.h"
#include "GTransform.h"

 void GMeshCollider::Awake()
 {
    m_transform = Obj()->Transform();
    m_phy_world = Obj()->PhyWorld();
    m_ground    = std::static_pointer_cast<btCollisionShape>(std::make_shared<btStaticPlaneShape>(btVector3(0,1,0),1));
    
    btRigidBody::btRigidBodyConstructionInfo rbinfo(0, this, m_ground.get(), btVector3(0,0,0));
    m_rigidbody = std::make_shared<btRigidBody>(rbinfo);

    m_phy_world.lock()->AddRigiBody(m_rigidbody);
}

void GMeshCollider::Exit()
{
    m_phy_world.lock()->DelRigiBody(m_rigidbody);
}

void GMeshCollider::getWorldTransform(btTransform& worldTrans ) const
{
    btVector3 pos(m_transform.lock()->postion.x, m_transform.lock()->postion.y, m_transform.lock()->postion.z);
    btQuaternion qua;
    qua.setEulerZYX(m_transform.lock()->rotate.z, m_transform.lock()->rotate.y, m_transform.lock()->rotate.x);
    worldTrans.setOrigin(pos);
    worldTrans.setRotation(qua);
}

void GMeshCollider::setWorldTransform(const btTransform& worldTrans)
{
    btTransform trans = worldTrans;
    btScalar rx, ry, rz;
    trans.getRotation().getEulerZYX(rz, ry, rx);
    m_transform.lock()->postion = glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
    m_transform.lock()->rotate  = glm::vec3(rx, ry, rz);
}