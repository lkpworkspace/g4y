#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "GObj.h"
#include "GCom.h"
#include "GScene.h"
#include "GCamera.h"
#include "GModel.h"
#include "GWorld.h"

class ModelScripts : public GCom
{
public:
    virtual void OnGUI() override
    {
        {
            static int counter = 0;

            ImGui::Begin("model setting");                          // Create a window called "Hello, world!" and append into it.

            ImGui::SliderFloat("model x", &x, -10.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("model y", &y, -10.0f, 10.0f);
            ImGui::SliderFloat("model z", &z, -10.0f, 10.0f);

            ImGui::SliderFloat("model rx", &rx, 0.0f, 180.0f);
            ImGui::SliderFloat("model ry", &ry, 0.0f, 180.0f);
            ImGui::SliderFloat("model rz", &rz, 0.0f, 180.0f);

            ImGui::End();
        }
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
};

void build_scene(std::shared_ptr<GScene> s)
{
    std::shared_ptr<GObj>  camera = std::make_shared<GObj>();
    std::shared_ptr<GObj>  model = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GCamera>()));

    model->AddDefaultComs();
    model->AddCom(std::static_pointer_cast<GCom>(std::make_shared<ModelScripts>()));
    model->AddCom(std::static_pointer_cast<GCom>(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj")));

    s->AddChild(camera);
    s->AddChild(model);
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
