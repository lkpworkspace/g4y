#ifndef __GPBCLIMSG_H__
#define __GPBCLIMSG_H__
#include <string>
#include "g4y.pb.h"
#include "GCom.h"
#include "GObj.h"
#include "GPbMsgFmt.h"
#include "GCliMsg.h"
#include "GPbCliMsgMgr.h"

class GPbCliTransformMsg : public GCliMsg<GPbCliMsgMgr, ::google::protobuf::Message>
{
public:
    GPbCliTransformMsg(bool local = true) :
        m_local(local)
    {}

    virtual void Awake() override
    {
        auto msg_mgr_obj = Obj()->FindWithTag("GPbCliMsgMgr");
        if(msg_mgr_obj != nullptr){
            // 创建序列化反序列化对象
            m_msg_fmt = std::make_shared<GPbMsgFmt>();
            // 获得transform组件
            m_transform = Obj()->GetCom<GTransform>("GTransform");
            // 获得cuuid
            SetCUUID(Obj()->UUID());
            // 将该消息对象加入到消息管理中
            m_msg_mgr = msg_mgr_obj->GetCom<GPbCliMsgMgr>("GPbCliMsgMgr");
            if(m_local){
                m_msg_mgr.lock()->AddLocalMsgObj(CUUID(), Obj());
                // 发送消息对象创建消息
                BroadcastCreateMsg();
                // 处于未激活状态
                // Obj()->SetActive(false);
            }else{
                m_msg_mgr.lock()->AddRemoteMsgObj(SUUID(), Obj());
            }
        }
    }

    virtual void Update() override
    {
        if(m_send_msg){
            using namespace ::google::protobuf;
            // 发送更新消息
            auto msg = std::make_shared<GTransformMsg>();
            glm::vec3 pos = m_transform.lock()->postion;
            glm::vec3 rot = m_transform.lock()->rotate;
            glm::vec3 scl = m_transform.lock()->scale;
            msg->set_cmd(GTransformMsg_Cmd::GTransformMsg_Cmd_UPDATE);
            msg->set_cuuid(m_client_id);
            msg->set_suuid(m_server_id);
            msg->set_posx(pos.x);
            msg->set_posy(pos.x);
            msg->set_posz(pos.x);
            msg->set_rx(rot.x);
            msg->set_ry(rot.y);
            msg->set_rz(rot.z);
            msg->set_sx(scl.x);
            msg->set_sy(scl.y);
            msg->set_sz(scl.z);
            SendMsg(msg);
            m_send_msg = !m_send_msg;
        }
    }

    virtual std::string ComName() override { return "GPbCliTransformMsg"; }

    /* 处理更新消息,被msgmanager调用 */
    virtual void UpdateMsg(std::shared_ptr<::google::protobuf::Message> msg) override
    {
        using namespace ::google::protobuf;
        auto tm = std::static_pointer_cast<GTransformMsg>(msg);
        if(tm->cmd() == GTransformMsg_Cmd::GTransformMsg_Cmd_UPDATE){
            m_transform.lock()->postion = glm::vec3(tm->posx(), tm->posy(), tm->posy());
            m_transform.lock()->rotate  = glm::vec3(tm->rx(), tm->ry(), tm->rz());
            m_transform.lock()->scale   = glm::vec3(tm->sx(), tm->sy(), tm->sz());
        }
    }

    /* 发送消息 */
    virtual void SendMsg(std::shared_ptr<::google::protobuf::Message> msg) override
    {
        m_msg_mgr.lock()->AddSendMsg(m_msg_fmt->Serilize(msg));
    }

    /* 广播创建消息 */
    void BroadcastCreateMsg()
    {
        using namespace ::google::protobuf;
        auto msg = std::make_shared<GTransformMsg>();
        glm::vec3 pos = m_transform.lock()->postion;
        glm::vec3 rot = m_transform.lock()->rotate;
        glm::vec3 scl = m_transform.lock()->scale;
        msg->set_cmd(GTransformMsg_Cmd::GTransformMsg_Cmd_CREATE);
        msg->set_cuuid(CUUID());
        msg->set_suuid(SUUID());
        msg->set_posx(pos.x);
        msg->set_posy(pos.x);
        msg->set_posz(pos.x);
        msg->set_rx(rot.x);
        msg->set_ry(rot.y);
        msg->set_rz(rot.z);
        msg->set_sx(scl.x);
        msg->set_sy(scl.y);
        msg->set_sz(scl.z);
        SendMsg(msg);
    }

    // for test
    void SendMsg2Server() { m_send_msg = true; }
    bool m_send_msg = false;

    std::shared_ptr<GPbMsgFmt> m_msg_fmt;
    std::weak_ptr<GTransform>  m_transform;

    bool m_local;
};

#endif