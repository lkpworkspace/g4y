#include "GCliMsgMgr.h"
#include "GCom.h"
#include "GObj.h"
#include "GMsg.h"

GCliMsgMgr::GCliMsgMgr()
{
    m_loc_objs.clear();
    m_srv_objs.clear();
    m_recv_msgs.clear();
    m_send_msgs.clear();
}

void GCliMsgMgr::PushMsg(std::string dat)
{
    // m_recv_msgs
    // 解析消息
    // 找到对应的对象
    // 给对象赋值
    // 缓存对象消息
    auto msgs = Deserilize(dat);
    auto meta_msg = std::static_pointer_cast<GMetaMsg>(msgs[0]);
    m_recv_msgs[meta_msg->srv_id()] = msgs;
}

std::string GCliMsgMgr::PopMsg()
{
    // m_send_msgs
    // 打包消息
    // 序列化
    // 从消息缓存中删除
    if(m_send_msgs.empty()) return std::string();

    std::string d = Serilize(m_send_msgs.begin()->second);
    m_send_msgs.erase(m_send_msgs.begin()->first);
    return d;
}

bool GCliMsgMgr::AddLocObj(std::string loc_id, std::shared_ptr<GObj> obj)
{
    if(m_loc_objs.find(loc_id) == m_loc_objs.end()){
        m_loc_objs[loc_id] = obj;
        return true;
    }
    return false;
}
void GCliMsgMgr::DelLocObj(std::string loc_id)
{
    if(m_loc_objs.find(loc_id) != m_loc_objs.end()){
        m_loc_objs.erase(loc_id);
    }
}

bool GCliMsgMgr::AddSrvObj(std::string srv_id, std::shared_ptr<GObj> obj)
{
    if(m_srv_objs.find(srv_id) == m_srv_objs.end()){
        m_srv_objs[srv_id] = obj;
        return true;
    }
    return false;
}
void GCliMsgMgr::DelSrvObj(std::string srv_id)
{
    if(m_srv_objs.find(srv_id) != m_srv_objs.end()){
        m_srv_objs.erase(srv_id);
    }
}

void GCliMsgMgr::Init()
{
    Obj()->SetTag("GCliMsgMgr");
}

/*
    - 接收到消息,判断是否存在在m_loc_objs中
        - 存在, 则时本地创建的对象, 找到该对象
        - 不存在, 在m_srv_objs中查找, 找到该对象
        - 都没有找到对象, 就创建一个对象,加入到m_srv_objs中
    - 判断消息类型
        - 是发送给Obj的消息(暂时不考虑)
            - 创建, 忽略
            - 更新, 忽略
            - 删除, 找到该对象, 从世界中删除
        - 是发送给组件的消息
            - 是创建, 则创建组件, 将消息发送给组件解析
            - 是更新, 将消息发送给组件解析
            - 是删除, 将消息发送给组件解析
*/
void GCliMsgMgr::Update()
{
    // 解析并分发
    for( const auto& p : m_recv_msgs ) {
        bool loc = false;
        auto msgs = p.second;
        auto meta = std::static_pointer_cast<GMetaMsg>(p.second[0]);
        std::shared_ptr<GObj> obj = nullptr;
        if(HaveLocObj(meta->loc_id())){
            loc = true;
            obj = m_loc_objs[meta->loc_id()].lock();
        }
        if(HaveSrvObj(meta->srv_id())){
            obj = m_srv_objs[meta->srv_id()].lock();
        }
        if(!obj){
            obj = std::make_shared<GObj>();
            m_srv_objs[meta->srv_id()] = obj;
            obj->AddDefaultComs();
            Obj()->Scene()->AddChild(obj);
        }
        for(int i = 0; i < meta->info_size(); ++i){
            auto com = obj->GetCom(msgs[i + 1]->GetTypeName());
            if(nullptr == com && !loc){
                auto msg_com = CreateCom<GMsg>(msgs[i + 1]->GetTypeName(), loc);
                msg_com->m_prt_loc_id = meta->parent_loc_id();
                msg_com->m_prt_srv_id = meta->parent_srv_id();
                msg_com->m_loc_id = meta->loc_id();
                msg_com->m_srv_id = meta->srv_id();
                obj->AddCom(msg_com);
                msg_com->ParseMsg(msgs[i + 1]);
                continue;
            }
            if(nullptr != com && loc){
                auto msg_com = std::static_pointer_cast<GMsg>(com);
                msg_com->ParseMsg(msgs[i + 1]);
                continue;
            }
            std::cout << "[error] unknown msg, skiped" << std::endl;
        }
    }
    m_recv_msgs.clear();

    // 仅用于调试
    for( const auto& msgs : m_send_msgs ) {
        for(const auto& msg : msgs.second){
            std::cout << msg->DebugString();
            std::cout << std::endl;
        }    
    }
    m_send_msgs.clear();
}


void GCliMsgMgr::Test()
{

}