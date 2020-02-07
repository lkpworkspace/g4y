#include "GPbTransformMsg.h"
#include "GObj.h"
#include "GTransform.h"
#include "GMsgMgr.h"
#include "GCliMsgMgr.h"

void GPbTransformMsg::Init()
{
    m_transform = Obj()->Transform();
    GMsg::Init();
}

void GPbTransformMsg::Update()
{
    static bool test = false;
    // 客户端本地创建的每个一段时间发送更新消息
    if(!test && m_loc && !m_is_srv){
        auto msg = BuildMsg();
        auto info = BuildInfoMsg(MsgName(), GMsgInfo_Action_UPDATE, msg->ByteSize());
        if(MsgMgr()){
            auto cli_msg_mgr = std::static_pointer_cast<GCliMsgMgr>(MsgMgr());
            cli_msg_mgr->BroadcastMsg(shared_from_this(), info, msg);
        }
        //test = true;
    }     
}

std::shared_ptr<::google::protobuf::Message> GPbTransformMsg::BuildMsg()
{
    using namespace ::google::protobuf;
    auto msg = std::make_shared<GTransformMsg>();

    glm::vec3 pos = m_transform.lock()->postion;
    glm::vec3 rot = m_transform.lock()->rotate;
    glm::vec3 scl = m_transform.lock()->scale;
    msg->set_posx(pos.x);
    msg->set_posy(pos.y);
    msg->set_posz(pos.z);
    msg->set_rx(rot.x);
    msg->set_ry(rot.y);
    msg->set_rz(rot.z);
    msg->set_sx(scl.x);
    msg->set_sy(scl.y);
    msg->set_sz(scl.z);
    return std::static_pointer_cast<Message>(msg);
}

// create/update/delete
void GPbTransformMsg::ParseMsg(std::shared_ptr<::google::protobuf::Message> msg)
{
    using namespace ::google::protobuf;
    auto tm = std::static_pointer_cast<GTransformMsg>(msg);
    m_transform.lock()->postion = glm::vec3(tm->posx(), tm->posy(), tm->posz());
    m_transform.lock()->rotate  = glm::vec3(tm->rx(), tm->ry(), tm->rz());
    m_transform.lock()->scale   = glm::vec3(tm->sx(), tm->sy(), tm->sz());
}


