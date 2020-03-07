#include "GRigibody.h"
#include "GCommon.h"

/*
    在Start中
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

        m_rigidbody->setAnisotropicFriction(btVector3(1,1,1));
        m_rigidbody->setRollingFriction(1);  // 滚动摩擦
        m_rigidbody->setSpinningFriction(1); // 旋转摩擦
        m_rigidbody->setHitFraction(1);

        m_rigidbody->setActivationState(DISABLE_DEACTIVATION);
        m_phy_world.lock()->AddRigiBody(m_rigidbody);
    }
}

void GRigibody::OnCollision(const btCollisionObject* col_obj)
{
    bool is_trigger = (m_rigidbody->getCollisionFlags() == btCollisionObject::CF_NO_CONTACT_RESPONSE);
    bool col_obj_exist = (m_last_cols.find(col_obj) != m_last_cols.end());

    // 获得所有组件并调用碰撞消息
    auto coms = Obj()->GetComs();
    for(const auto& c : coms){
        if(col_obj_exist){
            if(!is_trigger)
                c->OnCollisionStay();
            else
                c->OnTriggerStay();
        }else{
            if(!is_trigger)
                c->OnCollisionEnter();
            else
                c->OnTriggerEnter();
        }
    }
    m_cur_cols.insert(col_obj);
}

void GRigibody::OnCollisionEnd()
{
    bool is_trigger = (m_rigidbody->getCollisionFlags() == btCollisionObject::CF_NO_CONTACT_RESPONSE);
    auto coms = Obj()->GetComs();
    for(auto begin = m_last_cols.begin(); begin != m_last_cols.end(); ){
        if(m_cur_cols.find(*begin) == m_cur_cols.end()){
            for(const auto& c : coms){
                if(!is_trigger)
                    c->OnCollisionExit();
                else
                    c->OnTriggerExit();
            }
            begin = m_last_cols.erase(begin);
        }else{
            ++begin;
        }
    }
    m_last_cols.insert(m_cur_cols.begin(), m_cur_cols.end());
    m_cur_cols.clear();
}

void GRigibody::getWorldTransform(btTransform& worldTrans ) const
{
    auto p = m_transform.lock()->Position();
    auto q = m_transform.lock()->Rotation();
    worldTrans.setOrigin(btVector3(p.x, p.y, p.z));
    worldTrans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));
}

void GRigibody::setWorldTransform(const btTransform& worldTrans)
{
    btTransform trans = worldTrans;
    btScalar rx, ry, rz;
    trans.getRotation().getEulerZYX(rz, ry, rx);
    m_transform.lock()->SetPosition(glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()));
    m_transform.lock()->SetRotation(glm::vec3(rx, ry, rz));
}