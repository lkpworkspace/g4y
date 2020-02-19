#include "GCollider.h"
#include "GCommon.h"


/*
    Collider中OnCollision函数：
        由碰撞调用此函数，该函数处理碰撞，并将碰撞结果分发给该对象的的其他组件。
        
    派生类中：
        在Init函数中创建公共对象
            碰撞形状
            transform
            phy_world
        在Start函数中
            判断是否含有Rigibody组件
                有，不做任何事
                没有， 就创建一个碰撞对象加入物理世界
*/

void GCollider::OnCollision(const btCollisionObject* col_obj)
{
    // 判断该对象是否存在与上一次的碰撞，
    //  如果存在就调用OnCollisionStay函数
    //  如果不存在就调用OnCollisionEnter函数
    // 将碰撞对象加入到当前hash表中
    bool is_trigger = (m_col_obj->getCollisionFlags() == btCollisionObject::CF_NO_CONTACT_RESPONSE);
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

void GCollider::OnCollisionEnd()
{
    // 遍历上一次碰撞对象中 在这一次中是否存在
    // 如果存在，说明碰撞没有离开
    // 如果不存在， 则说明碰撞对象离开，调用OnCollisionExit函数， 并删除该对象
    // 将这一次的对象加入到上一次的hash表中，并去重复
    // 清空当前hash表
    bool is_trigger = (m_col_obj->getCollisionFlags() == btCollisionObject::CF_NO_CONTACT_RESPONSE);
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