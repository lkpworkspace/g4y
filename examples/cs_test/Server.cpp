#include "G4Y.h"
#include "GMsg.h"
#include "GPbTransformMsg.h"
#include "GMsgMgr.h"
#include "GSrvMsgMgr.h"
#include "GMsgNetIO.h"

/*
    测试1:
        - 创建服务端消息管理组件
        - 注册消息组件
        - 添加至场景

        - 创建服务端NETIO组件
        - 添加至场景
*/
class MoveScripts : public GCom
{
public:
    MoveScripts(std::string title) :
        title(title)
    {
    }

    virtual void Start() override
    {
        m_transform = Obj()->Transform();
    }

    virtual void Update() override
    {
        Obj()->Transform()->SetPosition(glm::vec3(x,y,z));
        Obj()->Transform()->SetRotation(glm::vec3(rx,ry,rz));
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
    auto  grid   = std::make_shared<GObj>();
    auto  netio  = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<MoveScripts>("Camera Setting"));

    auto msgmgr_com = std::make_shared<GSrvMsgMgr>();
    msgmgr_com->RegMsgCom("GTransformMsg", [](bool loc){ 
        return std::static_pointer_cast<GMsg>(std::make_shared<GPbTransformMsg>(loc)); 
    });
    msgmgr->AddDefaultComs();
    msgmgr->AddCom(msgmgr_com);

    auto netio_com = std::make_shared<GMsgNetIO>();
    netio_com->Start(true, "127.0.0.1", 6666);
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