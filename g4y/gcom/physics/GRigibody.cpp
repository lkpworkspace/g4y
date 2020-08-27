#include "GRigibody.h"
#include "GPhysics.hpp"
#include <btBulletDynamicsCommon.h>

#include "GWorld.h"
#include "GPhyWorld.h"
#include "GTransform.h"

static void get_world_transform(std::shared_ptr<GTransform> trans_com, btTransform& worldTrans)
{
	auto p = trans_com->Position();
	auto q = trans_com->Rotation();
	worldTrans.setOrigin(btVector3(p.x, p.y, p.z));
	worldTrans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));
}

static void set_world_transform(std::shared_ptr<GTransform> trans_com, const btTransform& worldTrans)
{
	btTransform trans = worldTrans;
	btScalar rx, ry, rz;
	trans.getRotation().getEulerZYX(rz, ry, rx);
	trans_com->SetPosition(glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()));
	trans_com->SetRotation(glm::vec3(rx, ry, rz));
}

GRigibody::GRigibody(GRigidBodyDes& des)
	: m_des(des)
{}

GRigibody::GRigibody(std::shared_ptr<GCollider> c)
	: m_des(c)
{}

/*
    在Start中
        寻找碰撞组件
            未找到就报错
            找到，创建rigibody组件，并加入世界
*/
void GRigibody::Init()
{
    m_transform = GetCom<GTransform>();
    m_phy_world = GWorld::Instance()->PhyWorld();
}

void GRigibody::Start()
{    
	assert(!m_des.shape.expired());
    auto collider = m_des.shape.lock();
    
	btVector3 inertia = g4y::convert_vec3_to_btvec3(m_des.local_inertia);
    collider->m_shape->calculateLocalInertia(m_des.mass, inertia);
	
	btTransform bttrans;
	get_world_transform(m_transform.lock(), bttrans);
	btDefaultMotionState* def_motion = new btDefaultMotionState(bttrans);
    btRigidBody::btRigidBodyConstructionInfo rbinfo(m_des.mass, def_motion, collider->m_shape.get(), inertia);
    m_rigidbody = new btRigidBody(rbinfo);
    m_rigidbody->setUserPointer(this);

	if(m_des.disable_sleep) m_rigidbody->setActivationState(DISABLE_DEACTIVATION);
    m_phy_world.lock()->AddRigiBody(m_rigidbody);
}

void GRigibody::OnDestroy()
{
	delete m_rigidbody->getMotionState();
	m_phy_world.lock()->DelRigiBody(m_rigidbody);
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

void GRigibody::SyncNodeToPhysics()
{
    auto motionState = m_rigidbody->getMotionState();

    btTransform trans;
    auto p = m_transform.lock()->Position();
    auto q = m_transform.lock()->Rotation();
    trans.setOrigin(btVector3(p.x, p.y, p.z));
    trans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));

    motionState->setWorldTransform(trans);
    m_rigidbody->setMotionState(motionState);
}

void GRigibody::SyncPhysicsToNode()
{
    btTransform trans = m_rigidbody->getWorldTransform();;
    btScalar rx, ry, rz;
    trans.getRotation().getEulerZYX(rz, ry, rx);
    m_transform.lock()->SetPosition(glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()));
    m_transform.lock()->SetRotation(glm::vec3(rx, ry, rz));
}