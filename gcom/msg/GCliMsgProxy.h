#ifndef __GCLIMSGPROXY_H__
#define __GCLIMSGPROXY_H__
#include <string>
#include <memory>
#include <tuple>
#include <vector>
#include <unordered_map>
#include "g4y.pb.h"

class GObj;
class GCliMsgProxy
{
public:
    GCliMsgProxy(unsigned int id) :
        m_id(id),
        m_recv_msg_cnt(0),
        m_process_recv_msg_cnt(0)
    {}
    virtual ~GCliMsgProxy(){}

    std::shared_ptr<GObj> MsgObj(std::string id);

    void AddObj(std::string id, std::shared_ptr<GObj>);
    void DelObj(std::string id);
    bool HaveObj(std::string id);

    unsigned int m_id;
    unsigned int m_recv_msg_cnt;
    unsigned int m_process_recv_msg_cnt;
    // loc id, obj
    std::unordered_map<std::string, std::weak_ptr<GObj>> m_msg_objs;

    // 发送消息缓存(loc id, msg: GMetaMsg在下标0, 其它均为组件消息)
    std::unordered_map<std::string, std::vector<std::shared_ptr<::google::protobuf::Message>>> m_send_msgs;
    // 接收消息缓存(srv id, msg: GMetaMsg在下标0, 其它均为组件消息)
    std::unordered_map<std::string, std::vector<std::shared_ptr<::google::protobuf::Message>>> m_recv_msgs;
};

#endif
