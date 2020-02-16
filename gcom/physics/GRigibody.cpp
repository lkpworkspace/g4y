#include "GRigibody.h"
#include "GCommon.h"

/*
    在Awake中
        寻找碰撞组件
            未找到就报错
            找到，创建rigibody组件，并加入世界
*/
void GRigibody::Init()
{
    m_transform = Obj()->Transform();
    m_phy_world = Obj()->PhyWorld();
}

void GRigibody::Start()
{    
    auto collider = Obj()->GetCom<GCollider>("GCollider");
    if(collider != nullptr){
        btScalar mass = 1;
        btVector3 inertia(0, 0, 0);
        collider->m_shape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rbinfo(mass, this, collider->m_shape.get(), inertia);
        m_rigidbody = std::make_shared<btRigidBody>(rbinfo);
        m_rigidbody->setUserPointer(this);
        m_rigidbody->setActivationState(DISABLE_DEACTIVATION);
        m_phy_world.lock()->AddRigiBody(m_rigidbody);
    }
}

void GRigibody::OnCollision()
{
    // 获得所有组件并调用碰撞消息
    auto coms = Obj()->GetComs();
    for(const auto& c : coms){
        c->OnCollisionStay();
    }
}

void GRigibody::getWorldTransform(btTransform& worldTrans ) const
{
    btVector3 pos(m_transform.lock()->postion.x, m_transform.lock()->postion.y, m_transform.lock()->postion.z);
    btQuaternion qua;
    qua.setEulerZYX(m_transform.lock()->rotate.z, m_transform.lock()->rotate.y, m_transform.lock()->rotate.x);
    worldTrans.setOrigin(pos);
    worldTrans.setRotation(qua);
}

void GRigibody::setWorldTransform(const btTransform& worldTrans)
{
    btTransform trans = worldTrans;
    btScalar rx, ry, rz;
    trans.getRotation().getEulerZYX(rz, ry, rx);
    m_transform.lock()->postion = glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
    m_transform.lock()->rotate  = glm::vec3(rx, ry, rz);
}