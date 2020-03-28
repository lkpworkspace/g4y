#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "GMsg.h"
#include "GMsgMgr.h"
#include "GCliMsgMgr.h"
#include "GPbTransformMsg.h"

/*
    模拟测试流程1:
        - 创建客户端消息管理组件
        - 注册消息组件
        - 添加至场景
        - 创建消息组件并添加至世界
        - 测试是否有创建消息广播出去
    模拟测试流程2:
        - 创建客户端消息管理组件
        - 注册消息组件
        - 添加至场景
        - 模拟服务器发送其他客户端创建对象消息
        - 测试客户端能否正常解析消息并创建对象
*/
class MsgScripts : public GCom
{
public:
    MsgScripts(std::string title) :
        title(title)
    {}

    virtual std::string ComName() override { return "MsgScripts"; }

    virtual void Start() override
    {
        auto msg_mgr_obj = GObj::FindWithTag("GCliMsgMgr");
        if(msg_mgr_obj){
            m_msgmgr = msg_mgr_obj->GetCom<GCliMsgMgr>("GCliMsgMgr");
            if(m_msgmgr.expired()){
                std::cout << "invalid msg mgr" << std::endl;
                return;
            }
        }else{
            std::cout << "can not find msgmgr obj" << std::endl;
        }
    }

    virtual void Update() override
    {
        ImGui::Begin(title.c_str());
        if(ImGui::Button("create loc msg com")){
            auto obj = std::make_shared<GObj>();
            auto msg = std::make_shared<GPbTransformMsg>(true);
            obj->AddDefaultComs();
            obj->Transform()->SetPosition(glm::vec3(10, 20, 0));
            obj->AddCom(msg);
            Obj()->Scene()->AddChild(obj);
        }
        if(ImGui::Button("create remote msg com")){
            auto obj = std::make_shared<GObj>();
            auto msg = std::make_shared<GPbTransformMsg>(false);
            obj->AddDefaultComs();
            obj->Transform()->SetPosition(glm::vec3(-10, 20, 0));
            obj->AddCom(msg);
            Obj()->Scene()->AddChild(obj);
        }
        if(ImGui::Button("sim server create msg com")){
            // build msg
            std::vector<std::shared_ptr<::google::protobuf::Message>> msgs;
            msgs.clear();
            auto obj = std::make_shared<GObj>();
            auto msg_com = std::make_shared<GPbTransformMsg>(false);
            msg_com->m_srv_id = "123456";
            obj->AddDefaultComs();
            obj->Transform()->SetPosition(glm::vec3(0, 20, 0));
            obj->AddCom(msg_com);
            auto meta = msg_com->BuildMetaMsg();
            auto msg = msg_com->BuildMsg();
            auto info = msg_com->BuildInfoMsg(msg_com->MsgName(), GMsgInfo_Action_CREATE, msg->ByteSize());
            auto meta_msg = std::static_pointer_cast<GMetaMsg>(meta);
            auto info_msg = std::static_pointer_cast<GMsgInfo>(info);
            auto info_ptr = meta_msg->add_info();
            info_ptr->set_action(info_msg->action());
            info_ptr->set_name(info_msg->name());
            info_ptr->set_sz(info_msg->sz());
            msgs.push_back(meta);
            msgs.push_back(msg);
            if(m_msgmgr.expired()){
                std::cout << "invaild msgmgr" << std::endl;
            }else{
                // serilize msg
                auto d = m_msgmgr.lock()->Serilize(msgs);
                // push msg
                m_msgmgr.lock()->PushMsg(d);
            }
        }

        ImGui::End();
    }
    std::string title;
    std::weak_ptr<GCliMsgMgr> m_msgmgr;
};

class CameraScripts : public GCom
{
public:
    CameraScripts(std::string title) :
        title(title)
    {
    }

    virtual void Start() override
    {
        m_transform = Obj()->Transform();
    }

    virtual void Update() override
    {
        ImGui::Begin(title.c_str());

        ImGui::SliderFloat("Camera x", &x, -150.0f, 150.0f);
        ImGui::SliderFloat("Camera y", &y, -150.0f, 150.0f);
        ImGui::SliderFloat("Camera z", &z, -150.0f, 150.0f);

        ImGui::SliderFloat("Camera rx", &rx, -180.0f, 180.0f);
        ImGui::SliderFloat("Camera ry", &ry, -180.0f, 180.0f);
        ImGui::SliderFloat("Camera rz", &rz, -180.0f, 180.0f);

        ImGui::End();
        Obj()->Transform()->SetPosition(glm::vec3(x,y,z));
        Obj()->Transform()->SetRotation(glm::vec3(rx,ry,rz));
    }

    float x = 0.0f;
    float y = 20.0f;
    float z = 30.0f;
    float rx = -20.0f;
    float ry = 0.0f;
    float rz = 0.0f;

    std::string title;
    std::weak_ptr<GTransform> m_transform;
};

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  msgmgr = std::make_shared<GObj>();
    auto  grid = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<CameraScripts>("Camera Setting"));

    auto msgmgr_com = std::make_shared<GCliMsgMgr>();
    msgmgr_com->RegMsgCom("GTransformMsg", [](bool loc){ 
        return std::static_pointer_cast<GMsg>(std::make_shared<GPbTransformMsg>(loc)); 
    });
    
    msgmgr->AddDefaultComs();
    msgmgr->AddCom(msgmgr_com);
    msgmgr->AddCom(std::make_shared<MsgScripts>("sim server msg"));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));
    
    s->AddChild(camera);
    s->AddChild(grid);
    s->AddChild(msgmgr);
}

int main(int argc, char** argv)
{
    const auto w = GWorld::Instance();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    w->SetScene(s);

    build_scene(s);

    return w->Run();
}