#include "GBoxCollider.h"
#include "GCommon.h"

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
   m_transform = Obj()->Transform();
   m_phy_world = GWorld::Instance()->PhyWorld();
   m_shape     = std::static_pointer_cast<btCollisionShape>(std::make_shared<btBoxShape>(btVector3(m_box_half_extents.x, m_box_half_extents.y, m_box_half_extents.z)));
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