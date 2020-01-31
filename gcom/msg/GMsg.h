#ifndef __GMSG_H__
#define __GMSG_H__
#include "g4y.pb.h"
#include "GCom.h"
enum class GMsgOperator : int {
    EMO_NONE,
    EMO_NORMAL,
    EMO_BOARDCAST,
    EMO_DESTORY,
};

class GCom;
class GMsgMgr;
class GMsg : public GCom
{
public:
    /*
    创建本地对象:
        1. 被创建时在Awake中查找GMsgMgr组件
        2. 将创建的初始化消息加入到GMsgMgr组件的发送消息队列中, 并将该对象放入到GMsgMgr的m_loc_objs中
        3. 在Update中隔一段时间将更新消息广播一次
        4. 在ParseMsg中解析从服务器收到的消息,并更新该组件
    创建非本地对象:
        1. 被创建时在Awake中查找GMsgMgr组件
        2. 将该对象放入到GMsgMge的m_srv_objs中
        3. 在ParseMsg中解析从服务器收到的消息,并更新该组件
    */
    GMsg(bool loc = true);

    virtual void Init() override;

    std::shared_ptr<GMsgMgr> MsgMgr() { return m_msg_mgr.expired() ? nullptr : m_msg_mgr.lock(); }

    // build GMetaMsg
    std::shared_ptr<::google::protobuf::Message> BuildMetaMsg();
    // build GMsgInfo
    std::shared_ptr<::google::protobuf::Message> BuildInfoMsg(std::string name, GMsgInfo_Action act, int sz);

    virtual std::shared_ptr<::google::protobuf::Message> BuildMsg() = 0;
    virtual void ParseMsg(std::shared_ptr<::google::protobuf::Message>) = 0;
    virtual std::string MsgName() = 0;

    std::string& LocId() { return m_loc_id; }
    std::string& SrvId() { return m_srv_id; }

    virtual std::string ComName() { return "GMsg"; }

    /* true: 服务端, false: 客户端 */
    bool m_is_srv;
    /* true: 本地客户端的对象, flase: 其它客户端的对象 */
    bool m_loc;
    /* 仅用于服务端 */
    unsigned int m_cli_id;
    std::string m_prt_loc_id;
    std::string m_prt_srv_id;
    std::string m_loc_id;
    std::string m_srv_id;
    std::weak_ptr<GMsgMgr> m_msg_mgr;
};

#endif
