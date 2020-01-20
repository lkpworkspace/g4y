#include "GSphereCollider.h"
#include "GObj.h"
#include "GTransform.h"

void GSphereCollider::Awake()
{
   m_transform = Obj()->Transform();
   m_phy_world = Obj()->PhyWorld();
   m_sphere       = std::static_pointer_cast<btCollisionShape>(std::make_shared<btSphereShape>(1));

   btScalar mass = 1;
   btVector3 inertia(0, 0, 0);
   m_sphere->calculateLocalInertia(mass, inertia);
   btRigidBody::btRigidBodyConstructionInfo rbinfo(mass, this, m_sphere.get(), inertia);
   m_rigidbody = std::make_shared<btRigidBody>(rbinfo);

   m_rigidbody->setActivationState(DISABLE_DEACTIVATION);
   m_phy_world.lock()->AddRigiBody(m_rigidbody);

}

void GSphereCollider::Exit()
{
   m_phy_world.lock()->DelRigiBody(m_rigidbody);
}

void GSphereCollider::getWorldTransform(btTransform& worldTrans ) const
{
   btVector3 pos(m_transform.lock()->postion.x, m_transform.lock()->postion.y, m_transform.lock()->postion.z);
   btQuaternion qua;
   qua.setEulerZYX(m_transform.lock()->rotate.z, m_transform.lock()->rotate.y, m_transform.lock()->rotate.x);
   worldTrans.setOrigin(pos);
   worldTrans.setRotation(qua);
}

void GSphereCollider::setWorldTransform(const btTransform& worldTrans)
{
   btTransform trans = worldTrans;
   btScalar rx, ry, rz;
   trans.getRotation().getEulerZYX(rz, ry, rx);
   m_transform.lock()->postion = glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
   m_transform.lock()->rotate  = glm::vec3(rx, ry, rz);
}