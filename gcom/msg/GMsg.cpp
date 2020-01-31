#include "GMsg.h"
#include "GObj.h"
#include "GCom.h"
#include "GMsgMgr.h"
#include "GCliMsgMgr.h"
#include "GSrvMsgMgr.h"
#include <iostream>

GMsg::GMsg(bool loc) :
        m_loc(loc),
        m_is_srv(false),
        m_prt_srv_id(""),
        m_prt_loc_id(""),
        m_srv_id(""),
        m_loc_id(""),
        m_cli_id(-1)
{
}

void GMsg::Init()
{
    /*
        查找到GCliMsgMgr, m_is_srv = false;
        查找到GSrvMsgMgr, m_is_srv = true;
        服务端:
            - 广播创建消息
        客户端:
            - 是本地对象
                - 发送创建消息
            - 不是本地对象
                - do nothing...
    */
   {
        auto msg_mgr_obj = GObj::FindWithTag("GCliMsgMgr");
        if(msg_mgr_obj){
            m_is_srv = false;
            m_loc_id = Obj()->UUID();
            m_msg_mgr = msg_mgr_obj->GetCom<GMsgMgr>("GCliMsgMgr");
            if(m_msg_mgr.expired()){
                std::cout << "invalid msg mgr" << std::endl;
                return;
            }
            auto cli_mgr = std::static_pointer_cast<GCliMsgMgr>(m_msg_mgr.lock());
            if(m_loc){
                auto msg = BuildMsg();
                auto info = BuildInfoMsg(MsgName(), GMsgInfo_Action_CREATE, msg->ByteSize());
                cli_mgr->BroadcastMsg(shared_from_this(), info, msg);
                cli_mgr->AddLocObj(m_loc_id, Obj());
            }else{
                cli_mgr->AddSrvObj(m_srv_id, Obj());
            }
            return;
        }else{
            std::cout << "[warning] can not find GCliMsgMgr com" << std::endl;
        }
   }
   {
        auto msg_mgr_obj = GObj::FindWithTag("GSrvMsgMgr");
        if(msg_mgr_obj){
            m_is_srv = true;
            m_srv_id = Obj()->UUID();
            m_msg_mgr = msg_mgr_obj->GetCom<GMsgMgr>("GSrvMsgMgr");
            if(m_msg_mgr.expired()){
                std::cout << "invalid msg mgr" << std::endl;
                return;
            }
            auto srv_mgr = std::static_pointer_cast<GSrvMsgMgr>(m_msg_mgr.lock());
            auto msg = BuildMsg();
            auto info = BuildInfoMsg(MsgName(), GMsgInfo_Action_CREATE, msg->ByteSize());
            srv_mgr->BroadcastMsg(shared_from_this(), info, msg);
        }else{
            std::cout << "[warning] can not find GSrvMsgMgr com" << std::endl;
        }
   }
}

std::shared_ptr<::google::protobuf::Message> GMsg::BuildMetaMsg()
{
    using namespace ::google::protobuf;
    auto meta = std::make_shared<GMetaMsg>();
    meta->set_parent_srv_id(m_prt_srv_id);
    meta->set_parent_loc_id(m_prt_loc_id);
    meta->set_srv_id(m_srv_id);
    meta->set_loc_id(m_loc_id);
    meta->clear_info();
    return std::static_pointer_cast<Message>(meta);
}

std::shared_ptr<::google::protobuf::Message> GMsg::BuildInfoMsg(std::string name, GMsgInfo_Action act, int sz)
{
    using namespace ::google::protobuf;
    auto info = std::make_shared<GMsgInfo>();
    info->set_action(act);
    info->set_name(name);
    info->set_sz(sz);
    return std::static_pointer_cast<Message>(info);
}

