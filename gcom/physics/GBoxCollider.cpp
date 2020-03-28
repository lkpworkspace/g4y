#include "GBoxCollider.h"
#include "GCommon.h"

void GBoxCollider::Init()
{
   m_transform = Obj()->Transform();
   m_phy_world = GWorld::Instance()->PhyWorld();
   m_shape     = std::static_pointer_cast<btCollisionShape>(std::make_shared<btBoxShape>(btVector3(btScalar(50.), btScalar(50.), btScalar(50.))));
}

void GBoxCollider::Start()
{
   auto com = Obj()->GetCom("GRigibody");
   if(com == nullptr){
      m_col_obj = std::make_shared<btCollisionObject>();
      m_col_obj->setUserPointer(this);
      m_col_obj->setCollisionShape(m_shape.get());
      m_phy_world.lock()->AddCollisionObj(m_col_obj);
   }
}

void GBoxCollider::OnDestroy()
{
   auto com = Obj()->GetCom("GRigibody");
   if(com == nullptr){
      m_phy_world.lock()->DelCollisionObj(m_col_obj);
   }
}