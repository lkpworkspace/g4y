#ifndef __GSRVMSGMGR_H__
#define __GSRVMSGMGR_H__
#include <string>
#include <memory>
#include <functional>
#include <queue>
#include <unordered_map>
#include "GMsgMgr.h"
#include "GCliMsgProxy.h"

/*
    1. 被创建后将所有预置好的消息组件注册到map中
        - 也可以通过加载配置文件的方式注册(没有反射,貌似行不通的样子-_-||)
    2. 设置该Obj的Tag为"GSrvMsgMgr"
    流程:
        服务端收到客户端消息
            - 找到对应的客户端代理
            - 未找到该对象
            - 创建对象
            - 根据消息创建组件
                - 赋值各种ID
                - 赋值客户端代理ID
                - 加入对象
                - 组件广播创建消息
        服务端广播消息
            - 搜集所有某个对象的组件消息
                - 打包广播
*/
class GSrvMsgMgr : public GMsgMgr
{
public:
    void PushMsg(unsigned int id, std::string);
    std::pair<unsigned int, std::string> PopMsg();

    void NewCli(unsigned int id);
    
    void Init();

    virtual void LateUpdate() override;

    virtual std::string ComName() { return "GSrvMsgMgr"; }

    /* msg com, GMsgInfo, GMetaMsg */
    template<typename T>
    void BroadcastMsg(std::shared_ptr<T>, std::shared_ptr<::google::protobuf::Message>, std::shared_ptr<::google::protobuf::Message>);

    void BroadcastMsg(unsigned int, std::string obj_loc_id, std::vector<std::shared_ptr<::google::protobuf::Message>>);
private:
    // 便于遍历
    std::vector<std::weak_ptr<GCliMsgProxy>> m_cli_proxy_vec;
    // 客户端代理对象, 便于搜索(cli id, cli proxy)
    std::unordered_map<unsigned int, std::shared_ptr<GCliMsgProxy>> m_cli_proxys;
};

/*
    将消息发送给所有客户端的的发送消息队列中(包含自己)
*/
template<typename T>
void GSrvMsgMgr::BroadcastMsg(std::shared_ptr<T> msg_com, std::shared_ptr<::google::protobuf::Message> info, std::shared_ptr<::google::protobuf::Message> msg)
{
    using namespace ::google::protobuf;
    auto com = std::static_pointer_cast<GMsg>(msg_com);
    for(const auto& p : m_cli_proxys){
        auto cli_id = p.first;
        auto cli    = p.second;
        auto& send_msgs = cli->m_send_msgs;

        if(com->m_cli_id == cli_id) continue;

        std::shared_ptr<Message> meta_msg;
        if(send_msgs.find(com->LocId()) == send_msgs.end()){
            // build meta msg
            meta_msg = com->BuildMetaMsg();
            std::vector<std::shared_ptr<Message>> msgs;
            msgs.push_back(meta_msg);
            send_msgs[com->LocId()] = msgs;
        }else{
            meta_msg = send_msgs[com->LocId()][0];
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
        std::vector<std::shared_ptr<Message>>& msgs = send_msgs[com->LocId()];
        msgs.push_back(msg);
    }
}

#endif