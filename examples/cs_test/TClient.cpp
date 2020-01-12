#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "GPbMsgFmt.h"
#include "GPbCliMsgMgr.h"
#include "GPbCliTransformMsg.h"

class MsgScripts : public GCom
{
public:
    MsgScripts(std::string title) :
        title(title)
    {}

    virtual std::string ComName() override { return "MsgScripts"; }

    virtual void Awake() override
    {
        // get msg mgr
        m_msg_mgr = Obj()->GetCom<GPbCliMsgMgr>("GPbCliMsgMgr");
        m_msg_fmt = std::make_shared<GPbMsgFmt>();
    }

    virtual void OnGUI() override
    {
        ImGui::Begin(title.c_str());
        if(ImGui::Button("create transform obj")){
            auto msg = std::make_shared<GTransformMsg>();
            msg->set_cmd(GTransformMsg_Cmd::GTransformMsg_Cmd_CREATE);
            msg->set_suuid("123456");
            msg->set_cuuid("");
            if(m_msg_mgr.expired()){
                std::cout << "error " << std::endl;
            }else{
                m_msg_mgr.lock()->PushMsg(m_msg_fmt->Serilize(msg));
            }
        }
        ImGui::End();
    }

    std::string title;
    std::weak_ptr<GPbCliMsgMgr> m_msg_mgr;
    std::shared_ptr<GPbMsgFmt> m_msg_fmt;
};

class CameraScripts : public GCom
{
public:
    CameraScripts(std::string title) :
        title(title)
    {
    }

    virtual void Awake() override
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
        ImGui::Begin(title.c_str());                          // Create a window called "Hello, world!" and append into it.

        ImGui::SliderFloat("Camera x", &x, -150.0f, 150.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("Camera y", &y, -150.0f, 150.0f);
        ImGui::SliderFloat("Camera z", &z, -150.0f, 150.0f);

        ImGui::SliderFloat("Camera rx", &rx, -180.0f, 180.0f);
        ImGui::SliderFloat("Camera ry", &ry, -180.0f, 180.0f);
        ImGui::SliderFloat("Camera rz", &rz, -180.0f, 180.0f);

        ImGui::End();
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

    msgmgr->AddDefaultComs();
    msgmgr->AddCom(std::make_shared<GPbCliMsgMgr>());
    msgmgr->AddCom<MsgScripts>(std::make_shared<MsgScripts>("sim server msg"));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));
    
    s->AddChild(camera);
    s->AddChild(grid);
    s->AddChild(msgmgr);
}

int main(int argc, char** argv)
{
    // build the world
    std::shared_ptr<GWorld> w = std::make_shared<GWorld>();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();
    w->SetScene(s);

    build_scene(s);
#ifdef USE_GUI
    while(!glfwWindowShouldClose(w->window)){
#else
    while(true){
#endif
        w->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}