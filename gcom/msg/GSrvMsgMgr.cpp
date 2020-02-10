#include "GSrvMsgMgr.h"
#include "GCommon.h"
#include "GMsgMgr.h"
#include "GMsg.h"

void GSrvMsgMgr::NewCli(unsigned int id)
{
    std::shared_ptr<GCliMsgProxy> cli = nullptr;
    if(m_cli_proxys.find(id) == m_cli_proxys.end()){
        cli = std::make_shared<GCliMsgProxy>(id);
        m_cli_proxy_vec.push_back(cli);
        m_cli_proxys[id] = cli;
    }
}

void GSrvMsgMgr::PushMsg(unsigned int id, std::string dat)
{
    BOOST_LOG_FUNCTION();
    static unsigned int cnt;
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
    BOOST_LOG_SEV(g_lg::get(), debug) << "cli " << id << " get msg " << ++cli->m_recv_msg_cnt;
    auto msgs = Deserilize(dat);
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
        if(cli.lock()->m_send_msgs.empty()) continue;
        BOOST_LOG_SEV(g_lg::get(), debug) << "cli " << cli.lock()->m_id << " pop msg -------------------------------------";
        std::string d = Serilize(cli.lock()->m_send_msgs.begin()->second);
        cli.lock()->m_send_msgs.erase(cli.lock()->m_send_msgs.begin()->first);
        return std::make_pair(cli.lock()->m_id, d);
    }
    return std::make_pair(-1, "");
}

void GSrvMsgMgr::BroadcastMsg(unsigned int id, std::string obj_loc_id, std::vector<std::shared_ptr<::google::protobuf::Message>> msgs)
{
    for(const auto& p : m_cli_proxys){
        auto cli_id = p.first;
        auto cli    = p.second;
        auto& send_msgs = cli->m_send_msgs;

        if(id == cli_id) continue;
        BOOST_LOG_SEV(g_lg::get(), debug) << "before forward cli " << id << " msg to cli " << cli_id << "-------------------------------------";
        send_msgs[obj_loc_id] = msgs;
    }
}

void GSrvMsgMgr::Init()
{
    Obj()->SetTag("GSrvMsgMgr");
}

void GSrvMsgMgr::LateUpdate()
{
    BOOST_LOG_FUNCTION();

    // 根据接收到的消息,创建或更新对象
    for(const auto& c : m_cli_proxy_vec){
        auto cli = c.lock();
        auto& recv_msgs = c.lock()->m_recv_msgs;
        if(recv_msgs.empty()) continue;
        cli->m_process_recv_msg_cnt += recv_msgs.size();
        BOOST_LOG_SEV(g_lg::get(), debug) << "cli " << cli->m_id << " process " << cli->m_process_recv_msg_cnt << " msg";
        for( const auto& p : recv_msgs ) {
            auto msgs = p.second;
            auto meta = std::static_pointer_cast<GMetaMsg>(p.second[0]);
            std::shared_ptr<GObj> obj = nullptr;
            if(cli->HaveObj(meta->loc_id())){
                obj = cli->MsgObj(meta->loc_id());
                // 转发消息给其他客户端
                BOOST_LOG_SEV(g_lg::get(), debug) << "cli " << cli->m_id << " forward msg -------------";
                meta->set_srv_id(obj->UUID());
                BroadcastMsg(cli->m_id, meta->loc_id(), msgs);
            }
            if(!obj){
                BOOST_LOG_SEV(g_lg::get(), debug) << "create obj " << meta->loc_id();
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