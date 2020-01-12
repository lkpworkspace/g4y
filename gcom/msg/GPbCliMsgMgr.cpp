#include "GPbCliMsgMgr.h"
#include "GPbCliTransformMsg.h"

void GPbCliMsgMgr::Update()
{
    /* 处理接收到的消息 */
    while(!m_recv_msgs.empty()){
        auto msg = m_msg_fmt->Deserilize(m_recv_msgs.front());
        std::string msg_name = msg->GetTypeName();
        std::cout << "recv msg: " << msg_name << std::endl;
        if(msg_name == "GTransformMsg"){
            auto tm = std::static_pointer_cast<GTransformMsg>(msg);
            if(m_local_msg_objs.find(tm->cuuid()) != m_local_msg_objs.end()){
                auto obj = m_local_msg_objs[tm->cuuid()].lock();
                auto com = obj->GetCom<GPbCliTransformMsg>("GPbCliTransformMsg");
                switch(tm->cmd()){
                case GTransformMsg_Cmd::GTransformMsg_Cmd_CREATE:{
                    com->SetSUUID(tm->suuid());
                    // 激活状态
                    // obj->SetActiove(true);
                    break;
                }
                case GTransformMsg_Cmd::GTransformMsg_Cmd_UPDATE:{
                    com->UpdateMsg(msg);
                    break;
                }
                case GTransformMsg_Cmd::GTransformMsg_Cmd_DELETE:{
                    std::cout << "[error] client get delete msg" << std::endl;
                    break;
                }
                }
            }else if(m_remote_msg_objs.find(tm->suuid()) != m_remote_msg_objs.end()){
                auto obj = m_remote_msg_objs[tm->suuid()].lock();
                auto com = obj->GetCom<GPbCliTransformMsg>("GPbCliTransformMsg");
                switch(tm->cmd()){
                case GTransformMsg_Cmd::GTransformMsg_Cmd_CREATE:{
                    std::cout << "[error] client get create msg" << std::endl;
                    break;
                }
                case GTransformMsg_Cmd::GTransformMsg_Cmd_UPDATE:{
                    com->UpdateMsg(msg);
                    break;
                }
                case GTransformMsg_Cmd::GTransformMsg_Cmd_DELETE:{
                    obj->Parent()->DelChild(obj);
                    break;
                }
                }
            }else{
                // create obj, msg com, add suuid
                auto obj = std::make_shared<GObj>();
                auto com = std::make_shared<GPbCliTransformMsg>(false);
                com->SetSUUID(tm->suuid());
                obj->AddDefaultComs();
                obj->AddCom(com);
                Obj()->Scene()->AddChild(obj);
                switch(tm->cmd()){
                case GTransformMsg_Cmd::GTransformMsg_Cmd_CREATE:{
                    std::cout << "[info] client create obj" << std::endl;
                    break;
                }
                case GTransformMsg_Cmd::GTransformMsg_Cmd_UPDATE:{
                    com->UpdateMsg(msg);
                    break;
                }
                case GTransformMsg_Cmd::GTransformMsg_Cmd_DELETE:{
                    std::cout << "[error] delete not exist msg obj" << std::endl;
                    break;
                }
                }
            }   
        }
        m_recv_msgs.pop();
    }
}