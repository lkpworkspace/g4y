#include "G4Y.h"
#include "GMsg.h"
#include "GPbTransformMsg.h"
#include "GMsgMgr.h"
#include "GSrvMsgMgr.h"
#include "GMsgNetIO.h"
#include "CameraScript.h"

/*
    测试1:
        - 创建服务端消息管理组件
        - 注册消息组件
        - 添加至场景

        - 创建服务端NETIO组件
        - 添加至场景
*/
void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  msgmgr = std::make_shared<GObj>();
    auto  grid   = std::make_shared<GObj>();
    auto  netio  = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<CameraScript>(glm::vec3(0, 20, 30)));

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
    GWorld::StaticInit();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    GWorld::s_instance->SetScene(s);

    build_scene(s);

    return GWorld::s_instance->Run();
}