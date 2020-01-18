#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"

class CameraScripts : public GCom
{
public:
    CameraScripts(std::string title) :
        title(title)
    {
    }

    virtual void Init() override
    {
        m_transform = Obj()->Transform();
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

    virtual void Update() override
    {
        Obj()->Transform()->postion = glm::vec3(x,y,z);
        Obj()->Transform()->rotate = glm::vec3(rx,ry,rz);
    }

    float x = 0.0f;
    float y = 60.0f;
    float z = 70.0f;
    float rx = -20.0f;
    float ry = 0.0f;
    float rz = 0.0f;

    std::string title;
    std::weak_ptr<GTransform> m_transform;
};

class ColliderScripts : public GCom
{
public:
    ColliderScripts(std::string title) :
        title(title)
    {}

    virtual void Awake() override
    {
        m_tranform = Obj()->Transform();
    }

    virtual void Update() override
    {
         ImGui::Begin(title.c_str());

        if(ImGui::Button("SetY")){
            m_tranform.lock()->postion = glm::vec3(0, 50, 0);
        }

        ImGui::End();
    }

    std::string title;
    std::weak_ptr<GTransform> m_tranform;
};

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  grid   = std::make_shared<GObj>();
    auto  ground = std::make_shared<GObj>();
    auto  sphere = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<CameraScripts>("Camera Setting"));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    ground->AddDefaultComs();
    ground->AddCom(std::make_shared<GMeshCollider>());

    sphere->AddDefaultComs();
    sphere->AddCom(std::make_shared<GSphereCollider>());
    sphere->AddCom(std::make_shared<ColliderScripts>("set sphere"));

    s->AddChild(camera);
    s->AddChild(grid);
    s->AddChild(ground);
    s->AddChild(sphere);
}

int main(int argc, char** argv)
{
    std::shared_ptr<GWorld> w = std::make_shared<GWorld>();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();
    
    w->SetScene(s);
    
    build_scene(s);

    return w->Run();
}
