#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"

class CameraScripts : public GCom
{
public:
    CameraScripts(std::string title) :
        title(title),
        z(15.0f)
    {
    }

    virtual void Init() override
    {
        m_transform = Obj()->Transform();
    }

    virtual void OnGUI() override
    {
        ImGui::Begin(title.c_str());                          // Create a window called "Hello, world!" and append into it.

        ImGui::SliderFloat("Camera x", &x, -10.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("Camera y", &y, -10.0f, 20.0f);
        ImGui::SliderFloat("Camera z", &z, -20.0f, 20.0f);

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

    float x;
    float y;
    float z;
    float rx;
    float ry;
    float rz;

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
    {

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
    float y = -8.0f;
    float z = -10.0f;
    float rx = 0.0f;
    float ry = 0.0f;
    float rz = 0.0f;

    std::string title;
};

void build_scene(std::shared_ptr<GScene> s)
{
    std::shared_ptr<GObj>  camera = std::make_shared<GObj>();
    std::shared_ptr<GObj>  model = std::make_shared<GObj>();
    std::shared_ptr<GObj>  model2 = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GCamera>()));
    camera->AddCom(std::static_pointer_cast<GCom>(std::make_shared<CameraScripts>("Camera Setting")));

    model->AddDefaultComs();
    model->AddCom(std::static_pointer_cast<GCom>(std::make_shared<ModelScripts>("Model1", glm::vec3(-5, -8, -10))));
    model->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj")));

    model2->AddDefaultComs();
    model2->AddCom(std::static_pointer_cast<GCom>(std::make_shared<ModelScripts>("Model2", glm::vec3(5, -8, -10))));
    model2->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj")));

    s->AddChild(camera);
    s->AddChild(model);
    s->AddChild(model2);
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
