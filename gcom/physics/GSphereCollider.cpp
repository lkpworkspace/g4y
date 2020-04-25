#include "GSphereCollider.h"
#include "GCommon.h"
#include "GRigibody.h"

void GSphereCollider::Init()
{
   m_transform = GetCom<GTransform>();
   m_phy_world = GWorld::Instance()->PhyWorld();
   m_shape     = std::static_pointer_cast<btCollisionShape>(std::make_shared<btSphereShape>(1));
}

void GSphereCollider::Start()
{
   auto com = GetCom<GRigibody>();
   if(com == nullptr){
      m_col_obj = std::make_shared<btCollisionObject>();
      m_col_obj->setUserPointer(this);
      m_col_obj->setCollisionShape(m_shape.get());
      m_phy_world.lock()->AddCollisionObj(m_col_obj);
   }
}

void GSphereCollider::OnDestroy()
{
	auto com = GetCom<GRigibody>();
	if (com == nullptr) {
		m_phy_world.lock()->DelCollisionObj(m_col_obj);
	}
}