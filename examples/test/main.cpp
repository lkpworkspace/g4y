#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>

#include "G4Y.h"
#include "RoamScript.h"

class TestCom : public GCom
{
    G_COM
public:
    TestCom(std::string name):
        m_name(name)
    {
        std::cout << "construct:" << m_name << std::endl;
    }

    virtual ~TestCom()
    {
        std::cout << "de construct:" << m_name << std::endl;
    }

    virtual void Update() override
    {
		// Call every frame
	}

    std::string m_name;
};

void build_scene(std::shared_ptr<GScene> s)
{
	auto camera = std::make_shared<GObj>();
	auto grid = std::make_shared<GObj>();

	camera->AddDefaultComs();
	camera->AddCom(std::make_shared<GCamera>());
	camera->AddCom(std::make_shared<RoamScript>());

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
