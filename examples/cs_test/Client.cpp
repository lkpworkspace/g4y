#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "GMsg.h"
#include "GMsgMgr.h"
#include "GCliMsgMgr.h"
#include "GPbTransformMsg.h"
#include "GMsgNetIO.h"

/*
    测试1: 创建一个本地消息对象
        - 创建客户端消息管理组件
        - 注册消息组件
        - 添加至场景

        - 创建客户端NETIO组件
        - 添加至场景
        
        - 点击按钮, 创建消息组件并添加至世界
*/
class MoveScripts : public GCom
{
public:
    MoveScripts(std::string title, glm::vec3 pos) :
        title(title),
        x(pos.x),
        y(pos.y),
        z(pos.z)
    {
    }

    virtual void Start() override
    {
        m_transform = Obj()->Transform();
    }

    virtual void Update() override
    {
        Obj()->Transform()->postion = glm::vec3(x,y,z);
        Obj()->Transform()->rotate = glm::vec3(rx,ry,rz);
    }

    virtual void OnGUI() override
    {
        ImGui::Begin(title.c_str());

        ImGui::SliderFloat("Move x", &x, -150.0f, 150.0f);
        ImGui::SliderFloat("Move y", &y, -150.0f, 150.0f);
        ImGui::SliderFloat("Move z", &z, -150.0f, 150.0f);

        ImGui::SliderFloat("Rotate x", &rx, -180.0f, 180.0f);
        ImGui::SliderFloat("Rotate y", &ry, -180.0f, 180.0f);
        ImGui::SliderFloat("Rotate z", &rz, -180.0f, 180.0f);

        ImGui::End();
    }

    float x;
    float y;
    float z;
    float rx = -20.0f;
    float ry = 0.0f;
    float rz = 0.0f;

    std::string title;
    std::weak_ptr<GTransform> m_transform;
};

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

    virtual void OnGUI() override
    {
        static int cnt = 0;
        ImGui::Begin(title.c_str());
        if(ImGui::Button("Create msg com")){
            auto obj = std::make_shared<GObj>();
            auto msg = std::make_shared<GPbTransformMsg>(true);
            auto move_srcipts = std::make_shared<MoveScripts>("move" + std::to_string(cnt++), glm::vec3(0, 0, 0));
            obj->AddDefaultComs();
            obj->AddCom(msg);
            obj->AddCom(move_srcipts);
            Obj()->Scene()->AddChild(obj);
        }

        ImGui::End();
    }
    std::string title;
    std::weak_ptr<GCliMsgMgr> m_msgmgr;
};

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  msgmgr = std::make_shared<GObj>();
    auto  grid   = std::make_shared<GObj>();
    auto  netio  = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<MoveScripts>("Camera Setting", glm::vec3(0, 20, 30)));

    auto msgmgr_com = std::make_shared<GCliMsgMgr>();
    msgmgr_com->RegMsgCom("GTransformMsg", [](bool loc){ 
        return std::static_pointer_cast<GMsg>(std::make_shared<GPbTransformMsg>(loc)); 
    });
    
    msgmgr->AddDefaultComs();
    msgmgr->AddCom(msgmgr_com);
    msgmgr->AddCom(std::make_shared<MsgScripts>("sim server msg"));

    auto netio_com = std::make_shared<GMsgNetIO>();
    netio_com->Start(false, "127.0.0.1", 6666);
    netio->AddDefaultComs();
    netio->AddCom(netio_com);

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));
    
    s->AddChild(camera);
    s->AddChild(grid);
    s->AddChild(msgmgr);
    s->AddChild(netio);
}

int main(int argc, char** argv)
{
    std::shared_ptr<GWorld> w = std::make_shared<GWorld>();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();
    
    w->SetScene(s);

    build_scene(s);

    return w->Run();
}