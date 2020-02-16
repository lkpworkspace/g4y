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
        在Awake函数中
            判断是否含有Rigibody组件
                有，不做任何事
                没有， 就创建一个碰撞对象加入物理世界
*/

void GCollider::OnCollision()
{
    // 获得所有组件并调用碰撞消息
    auto coms = Obj()->GetComs();
    for(const auto& c : coms){
        c->OnCollisionStay();
    }
}