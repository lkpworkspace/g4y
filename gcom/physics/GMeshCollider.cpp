#include "GMeshCollider.h"
#include "GCommon.h"

void GMeshCollider::Init()
{
    m_transform = Obj()->Transform();
    m_phy_world = Obj()->PhyWorld();
    m_shape     = std::static_pointer_cast<btCollisionShape>(std::make_shared<btStaticPlaneShape>(btVector3(0,1,0),1));
}

void GMeshCollider::Start()
{    
    auto com = Obj()->GetCom("GRigibody");
    if(com == nullptr){
        m_col_obj = std::make_shared<btCollisionObject>();
        m_col_obj->setUserPointer(this);
        m_col_obj->setCollisionShape(m_shape.get());
        m_col_obj->setAnisotropicFriction(btVector3(1,1,1));
        m_col_obj->setRollingFriction(1);  // 滚动摩擦
        m_col_obj->setSpinningFriction(1); // 旋转摩擦
        m_col_obj->setHitFraction(1);
        m_phy_world.lock()->AddCollisionObj(m_col_obj);
    }
}

void GMeshCollider::Exit()
{
    m_phy_world.lock()->DelCollisionObj(m_col_obj);
}
