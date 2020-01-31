#ifndef __GCLIMSGMGR_H__
#define __GCLIMSGMGR_H__
#include "GMsgMgr.h"
#include "g4y.pb.h"
#include <iostream>
class GMsg;
class GCom;
class GObj;
class GMsgMgr;
/*
    1. 被创建后将所有预置好的消息组件注册到map中
        - 也可以通过加载配置文件的方式注册(没得反射,貌似行不通的样子-_-||)
    2. 设置该Obj的Tag为"GCliMsgMgr"
*/
class GCliMsgMgr : public GMsgMgr
{
public:
    GCliMsgMgr();
    /* 被传输消息协议调用 */
    void PushMsg(std::string);
    std::string PopMsg();

    /* 添加/删除本地创建的对象(loc id, obj) */
    bool AddLocObj(std::string, std::shared_ptr<GObj>);
    void DelLocObj(std::string);
    bool HaveLocObj(std::string loc_id) { return m_loc_objs.find(loc_id) != m_loc_objs.end(); }

    /* 添加/删除其它客户端创建的对象(srv id, obj) */
    bool AddSrvObj(std::string, std::shared_ptr<GObj>);
    void DelSrvObj(std::string);
    bool HaveSrvObj(std::string srv_id){ return m_srv_objs.find(srv_id) != m_srv_objs.end(); }

    virtual void Init() override;

    virtual void Update() override;

    virtual std::string ComName() { return "GCliMsgMgr"; }

    /* 仅用于测试该类的成员方法 */
    static void Test();

    /* msg com, GMsgInfo, GMetaMsg */
    template<typename T>
    void BroadcastMsg(std::shared_ptr<T>, std::shared_ptr<::google::protobuf::Message>, std::shared_ptr<::google::protobuf::Message>);

private:
    // 本地消息对象(loc id, obj)
    std::unordered_map<std::string, std::weak_ptr<GObj>> m_loc_objs;
    // 服务端消息对象(srv id, obj)
    std::unordered_map<std::string, std::weak_ptr<GObj>> m_srv_objs;

    // 发送消息缓存(loc id, msg: GMetaMsg在下标0, 其它均为组件消息)
    std::unordered_map<std::string, std::vector<std::shared_ptr<::google::protobuf::Message>>> m_send_msgs;
    // 接收消息缓存(srv id, msg: GMetaMsg在下标0, 其它均为组件消息)
    std::unordered_map<std::string, std::vector<std::shared_ptr<::google::protobuf::Message>>> m_recv_msgs;
};

template<typename T>
void GCliMsgMgr::BroadcastMsg(std::shared_ptr<T> msg_com, std::shared_ptr<::google::protobuf::Message> info, std::shared_ptr<::google::protobuf::Message> msg)
{
    using namespace ::google::protobuf;
    auto com = std::static_pointer_cast<GMsg>(msg_com);
    std::shared_ptr<Message> meta_msg;
    if(m_send_msgs.find(com->LocId()) == m_send_msgs.end()){
        // build meta msg
        meta_msg = com->BuildMetaMsg();
        std::vector<std::shared_ptr<Message>> msgs;
        msgs.push_back(meta_msg);
        m_send_msgs[com->LocId()] = msgs;
    }else{
        meta_msg = m_send_msgs[com->LocId()][0];
        // update meta msg???
    }
    // add msg info
    auto meta = std::static_pointer_cast<GMetaMsg>(meta_msg);
    auto info2 = std::static_pointer_cast<GMsgInfo>(info);
    auto info_ptr = meta->add_info();
    info_ptr->set_action(info2->action());
    info_ptr->set_name(info2->name());
    info_ptr->set_sz(info2->sz());

    // add com msg
    std::vector<std::shared_ptr<Message>>& msgs = m_send_msgs[com->LocId()];
    msgs.push_back(msg);
}


#endif