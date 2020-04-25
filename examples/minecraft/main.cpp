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
    g4y::resourcemgr()->SetResourceDir("D:/projects/g4y/asset/");
	for (int i = 0; i < 37; ++i) {
		std::string model_name = "MC/" + std::to_string(i + 1) + ".obj";
		g4y::resourcemgr()->LoadModel(model_name);
	}
}

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  grid = std::make_shared<GObj>();
	auto  player_obj = std::make_shared<GObj>();

	player_obj->AddCom(std::make_shared<Player>());

    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<RoamScript>("Camera roam"));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    s->AddChild(camera);
    s->AddChild(grid);

	/*auto mc_block = g4y::resourcemgr()->Instantiate("MC/polySurface178.obj");
	mc_block->Transform()->SetPosition(glm::vec3(0, 0, 0));
	s->AddChild(mc_block);*/

	s->AddChild(player_obj);
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
