#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>

#include "G4Y.h"
#include "RoamScript.h"
#include "ShowImGuiDemo.h"

void build_scene(std::shared_ptr<GScene> s)
{
	auto camera = std::make_shared<GObj>();
	auto grid = std::make_shared<GObj>();

	camera->AddDefaultComs();
	camera->AddCom(std::make_shared<GCamera>());
	camera->AddCom(std::make_shared<RoamScript>());
	camera->AddCom(std::make_shared<ShowImGuiDemoScript>());

	grid->AddDefaultComs();
	grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));
	grid->AddCom(std::make_shared<GAxis>());

	s->AddChild(camera);
	s->AddChild(grid);
}

int main(int argc, char** argv)
{
    const auto w = GWorld::Instance();
	w->Init(argc, argv);

    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    w->SetScene(s);

    build_scene(s);

    return w->Run();
}
