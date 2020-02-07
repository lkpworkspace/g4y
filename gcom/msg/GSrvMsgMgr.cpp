#include "GSrvMsgMgr.h"
#include "GCom.h"
#include "GMsgMgr.h"
#include "GObj.h"
#include "GMsg.h"

void GSrvMsgMgr::PushMsg(unsigned int id, std::string dat)
{
    // 找到客户端代理
    // 未找到就创建
    // 反序列化
    // 将消息缓存到接受消息容器中
    std::shared_ptr<GCliMsgProxy> cli = nullptr;
    if(m_cli_proxys.find(id) == m_cli_proxys.end()){
        cli = std::make_shared<GCliMsgProxy>(id);
        m_cli_proxy_vec.push_back(cli);
        m_cli_proxys[id] = cli;
    }else{
        cli = m_cli_proxys[id];
    }
    auto msgs = Deserilize(dat);
    // for(int i = 0; i < msgs.size(); ++i){
    //     std::cout << msgs[i]->DebugString() << std::endl;
    // }
    auto meta_msg = std::static_pointer_cast<GMetaMsg>(msgs[0]);
    cli->m_recv_msgs[meta_msg->loc_id()] = msgs;
}

std::pair<unsigned int, std::string> GSrvMsgMgr::PopMsg()
{
    // 找到客户端代理
    // 找到一条消息
    // 序列化
    // 删除该消息
    // 返回序列化的消息
    for(int i = 0; i < m_cli_proxy_vec.size(); ++i){
        auto cli = m_cli_proxy_vec[i];
        if(cli.lock()->m_send_msgs.empty()) return std::make_pair(-1, "");

        std::string d = Serilize(cli.lock()->m_send_msgs.begin()->second);
        cli.lock()->m_send_msgs.erase(cli.lock()->m_send_msgs.begin()->first);
        return std::make_pair(cli.lock()->m_id, d);
    }
    return std::make_pair(-1, "");
}

void GSrvMsgMgr::Init()
{
    Obj()->SetTag("GSrvMsgMgr");
}

void GSrvMsgMgr::LateUpdate()
{
    // 根据接收到的消息,更新对象
    // 解析并分发
    for(const auto& c : m_cli_proxy_vec){
        auto cli = c.lock();
        auto& recv_msgs = c.lock()->m_recv_msgs;
        if(recv_msgs.empty()) continue;
        for( const auto& p : recv_msgs ) {
            auto msgs = p.second;
            auto meta = std::static_pointer_cast<GMetaMsg>(p.second[0]);
            std::shared_ptr<GObj> obj = nullptr;
            if(cli->HaveObj(meta->loc_id())){
                obj = cli->MsgObj(meta->loc_id());
            }
            if(!obj){
                obj = std::make_shared<GObj>();
                cli->AddObj(meta->loc_id(), obj);
                obj->AddDefaultComs();
                Obj()->Scene()->AddChild(obj);
            }
            for(int i = 0; i < meta->info_size(); ++i){
                auto com = obj->GetCom(msgs[i + 1]->GetTypeName());
                if(nullptr == com){
                    auto msg_com = CreateCom<GMsg>(msgs[i + 1]->GetTypeName(), false);
                    msg_com->m_prt_loc_id = meta->parent_loc_id();
                    msg_com->m_prt_srv_id = meta->parent_srv_id();
                    msg_com->m_loc_id = meta->loc_id();
                    // msg_com->m_srv_id = meta->srv_id();
                    msg_com->m_cli_id = cli->m_id;
                    obj->AddCom(msg_com);
                    msg_com->ParseMsg(msgs[i + 1]);
                    continue;
                }else{
                    auto msg_com = std::static_pointer_cast<GMsg>(com);
                    msg_com->ParseMsg(msgs[i + 1]);
                    continue;
                }
                std::cout << "[error] unknown msg, skiped" << std::endl;
            }
        }
        recv_msgs.clear();
    }
}