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
    float y = 20.0f;
    float z = 30.0f;
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
    }

    virtual void OnGUI() override
    {
        ImGui::Begin(title.c_str());                          // Create a window called "Hello, world!" and append into it.

        ImGui::SliderFloat("model x", &x, -10.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("model y", &y, -10.0f, 10.0f);
        ImGui::SliderFloat("model z", &z, -10.0f, 10.0f);

        ImGui::SliderFloat("model rx", &rx, 0.0f, 180.0f);
        ImGui::SliderFloat("model ry", &ry, -180.0f, 180.0f);
        ImGui::SliderFloat("model rz", &rz, 0.0f, 180.0f);

        ry = std::cos(glfwGetTime()) * 180;
        ImGui::End();
    }

    virtual void Update() override
    {
        glm::mat4 m(1.0f);
        
        m = glm::translate(m, glm::vec3(x, y, z));
        m = glm::rotate(m, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
        m = glm::rotate(m, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
        m = glm::rotate(m, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
        Obj()->Transform()->model = m;
    }

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float rx = 0.0f;
    float ry = 0.0f;
    float rz = 0.0f;

    std::string title;
};

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  model = std::make_shared<GObj>();
    auto  model2 = std::make_shared<GObj>();
    auto  model3 = std::make_shared<GObj>();
    auto  grid = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GCamera>()));
    camera->AddCom(std::static_pointer_cast<GCom>(std::make_shared<CameraScripts>("Camera Setting")));

    model->AddDefaultComs();
    model->AddCom(std::static_pointer_cast<GCom>(std::make_shared<ModelScripts>("Model1", glm::vec3(-10, 0, -10))));
    model->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj")));

    model2->AddDefaultComs();
    model2->AddCom(std::static_pointer_cast<GCom>(std::make_shared<ModelScripts>("Model2", glm::vec3(0, 0, 0))));
    model2->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj")));

    model3->AddDefaultComs();
    model3->AddCom(std::static_pointer_cast<GCom>(std::make_shared<ModelScripts>("Model3", glm::vec3(10, 0, -10))));
    model3->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj")));

    grid->AddDefaultComs();
    grid->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GGrid>(-100, 100, 1)));
    
    s->AddChild(camera);
    s->AddChild(grid);
    s->AddChild(model);
    s->AddChild(model2);
    s->AddChild(model3);
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
