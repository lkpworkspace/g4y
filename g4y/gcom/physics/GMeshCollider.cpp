#include "GMeshCollider.h"
#include <btBulletDynamicsCommon.h>
#include "GWorld.h"
#include "GPhyWorld.h"
#include "GRigibody.h"
#include "GTransform.h"

void GMeshCollider::Init()
{
    m_transform = GetCom<GTransform>();
    m_phy_world = GWorld::Instance()->PhyWorld();
    m_shape     = std::static_pointer_cast<btCollisionShape>(std::make_shared<btStaticPlaneShape>(btVector3(0,1,0),0));
}

void GMeshCollider::Start()
{    
    auto com = GetCom<GRigibody>();
    if(com == nullptr){
        m_col_obj = std::make_shared<btCollisionObject>();
        m_col_obj->setUserPointer(this);
        m_col_obj->setCollisionShape(m_shape.get());
        m_phy_world.lock()->AddCollisionObj(m_col_obj);
    }
}

void GMeshCollider::OnDestroy()
{
	auto com = GetCom<GRigibody>();
	if (com == nullptr) {
		m_phy_world.lock()->DelCollisionObj(m_col_obj);
	}
}
