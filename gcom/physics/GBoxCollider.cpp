#include "GBoxCollider.h"
#include "GCommon.h"
#include "GRigibody.h"

GBoxCollider::GBoxCollider() :
   m_box_half_extents(.5, .5, .5)
{}

GBoxCollider::GBoxCollider(glm::vec3 box_half_extents) :
   m_box_half_extents(box_half_extents.x, box_half_extents.y, box_half_extents.z)
{}

void GBoxCollider::SetBoxHalfExtents(glm::vec3 v)
{
   m_box_half_extents = v;
   auto box_shape = std::static_pointer_cast<btBoxShape>(m_shape);
}
glm::vec3 GBoxCollider::GetBoxHalfExtents()
{
   return m_box_half_extents;
}

void GBoxCollider::Init()
{
   m_transform = GetCom<GTransform>();
   m_phy_world = GWorld::Instance()->PhyWorld();
   m_shape     = std::static_pointer_cast<btCollisionShape>(std::make_shared<btBoxShape>(btVector3(m_box_half_extents.x, m_box_half_extents.y, m_box_half_extents.z)));
}

void GBoxCollider::Start()
{
   auto com = GetCom<GRigibody>();
   if(com == nullptr){
	  m_has_rigibody = false;
      m_col_obj = std::make_shared<btCollisionObject>();
      m_col_obj->setUserPointer(this);
      m_col_obj->setCollisionShape(m_shape.get());
	  SetPostion(m_transform.lock()->Position());
      m_phy_world.lock()->AddCollisionObj(m_col_obj);
   }
   else {
	   m_has_rigibody = true;
   }
}

void GBoxCollider::Update()
{
	if (!m_has_rigibody) {
		auto pos = m_col_obj->getWorldTransform().getOrigin();
		m_transform.lock()->SetPosition(glm::vec3(pos[0], pos[1], pos[2]));
	}
}

void GBoxCollider::OnDestroy()
{
	auto com = GetCom<GRigibody>();
	if (com == nullptr) {
		m_phy_world.lock()->DelCollisionObj(m_col_obj);
	}
}