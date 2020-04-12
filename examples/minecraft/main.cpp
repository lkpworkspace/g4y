#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "Player.hpp"
#include "Block.hpp"

#include "RoamScript.h"
#include "CameraScript.h"
#include "BulletScript.h"
#include "MoveScript.h"

void load_resource(std::shared_ptr<GScene> s)
{
    g4y::resourcemgr()->SetResourceDir("/home/lkp/projs/g4y/asset/");
    g4y::resourcemgr()->LoadModel("MC2/MCblocks.obj");
}

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  grid = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<RoamScript>("Camera roam"));
    camera->AddCom(std::make_shared<Player>());
    camera->AddCom(std::make_shared<GSkybox>(
        std::vector<std::string>({
            "/home/lkp/projs/gfy/build/skybox/right.jpg",
            "/home/lkp/projs/gfy/build/skybox/left.jpg",
            "/home/lkp/projs/gfy/build/skybox/top.jpg",
            "/home/lkp/projs/gfy/build/skybox/bottom.jpg",
            "/home/lkp/projs/gfy/build/skybox/front.jpg",
            "/home/lkp/projs/gfy/build/skybox/back.jpg",
        })
    ));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    s->AddChild(camera);
    s->AddChild(grid);
}

int main(int argc, char** argv)
{
    const auto w = GWorld::Instance();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    w->SetScene(s);

    load_resource(s);
    build_scene(s);

    return w->Run();
}
