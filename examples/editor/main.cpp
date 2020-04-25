#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "Panel.h"
#include "PanelMgr.h"
#include "RoamScript.h"

class ScenePanel : public Panel, public std::enable_shared_from_this<ScenePanel>
{
public:

    virtual std::string PanelName() override
    {
        return "Scene";
    }

    virtual std::shared_ptr<Panel> Clone() override
    {
        return nullptr;
    }

    virtual void Resize(glm::ivec4 rect) override
    {
        g4y::glview()->SetRenderRect(rect);
    }
};

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  grid = std::make_shared<GObj>();
    auto  panelmgr = std::make_shared<GObj>();
    auto  scenepanel = std::make_shared<GObj>();

    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<RoamScript>());

    auto scenepanel_com = std::make_shared<ScenePanel>();
    panelmgr->AddCom(scenepanel_com);

    auto panelmgr_com = std::make_shared<PanelMgr>(); 
    panelmgr->SetTag("PanelMgr");
    panelmgr->AddCom(panelmgr_com);
    panelmgr_com->RegPanel(scenepanel_com);

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    s->AddChild(camera);
    s->AddChild(grid);
    s->AddChild(panelmgr);

    g4y::glview()->ShowDemo(false);
}

int main(int argc, char** argv)
{
    const auto w = GWorld::Instance();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    w->SetScene(s);

    build_scene(s);

    return w->Run();

    // PanelMgr::Test();
}
