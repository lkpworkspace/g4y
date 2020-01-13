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

class ModelScripts : public GCom
{
public:
    ModelScripts(std::string title, glm::vec3 pos) :
        title(title),
        x(pos.x),
        y(pos.y),
        z(pos.z)
    {}

    virtual void Awake() override
    {
        std::cout << "Model Scripts awake" << std::endl;
        m_tranform = Obj()->Transform();
    }

    virtual void OnGUI() override
    {
        ImGui::Begin(title.c_str());

        ry = std::cos(glfwGetTime()) * 180;

        ImGui::SliderFloat("model x", &x, -10.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("model y", &y, -10.0f, 10.0f);
        ImGui::SliderFloat("model z", &z, -10.0f, 10.0f);

        ImGui::SliderFloat("model rx", &rx, 0.0f, 180.0f);
        ImGui::SliderFloat("model ry", &ry, -180.0f, 180.0f);
        ImGui::SliderFloat("model rz", &rz, 0.0f, 180.0f);

        ImGui::SliderFloat("model sx", &sx, 0.0f, 1.0f);
        ImGui::SliderFloat("model sy", &sy, 0.0f, 1.0f);
        ImGui::SliderFloat("model sz", &sz, 0.0f, 1.0f);

        ImGui::End();
    }

    virtual void Update() override
    {
        m_tranform.lock()->postion = glm::vec3(x, y, z);
        m_tranform.lock()->rotate  = glm::vec3(rx, ry, rz);
        m_tranform.lock()->scale   = glm::vec3(sx, sy, sz);  
    }

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float rx = 0.0f;
    float ry = 0.0f;
    float rz = 0.0f;
    float sx = 1.0f;
    float sy = 1.0f;
    float sz = 1.0f;

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
    camera->AddCom(std::make_shared<GSkybox>(
        std::vector<std::string>({
            "/home/lkp/projs/gfy/build/skybox/right.jpg",
            "/home/lkp/projs/gfy/build/skybox/left.jpg",
            "/home/lkp/projs/gfy/build/skybox/top.jpg",
            "/home/lkp/projs/gfy/build/skybox/bottom.jpg",
            "/home/lkp/projs/gfy/build/skybox/front.jpg",
            "/home/lkp/projs/gfy/build/skybox/back.jpg",
            // "/home/lkp/projs/gfy/build/skybox3/perdicus_rt.tga",
            // "/home/lkp/projs/gfy/build/skybox3/perdicus_lf.tga",
            // "/home/lkp/projs/gfy/build/skybox3/perdicus_up.tga",
            // "/home/lkp/projs/gfy/build/skybox3/perdicus_dn.tga",
            // "/home/lkp/projs/gfy/build/skybox3/perdicus_bk.tga",
            // "/home/lkp/projs/gfy/build/skybox3/perdicus_ft.tga",
        })
    ));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    ground->AddDefaultComs();
    ground->AddCom(std::make_shared<GGround>());

    sphere->AddDefaultComs();
    sphere->AddCom(std::make_shared<GSphere>());

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
