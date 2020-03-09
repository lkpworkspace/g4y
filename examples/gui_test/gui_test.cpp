#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "CameraScripts.h"

class ModelScripts : public GCom
{
public:
    ModelScripts(std::string title, glm::vec3 pos) :
        title(title),
        x(pos.x),
        y(pos.y),
        z(pos.z)
    {}

    virtual void Start() override
    {
        std::cout << "Model scripts start" << std::endl;
        m_tranform = Obj()->Transform();
        m_tranform.lock()->SetPosition(glm::vec3(0, 1, 0));
    }

    virtual void Update() override
    {
        ImGui::Begin(title.c_str());

        bool slider_rotate[3];
        slider_rotate[0] = ImGui::SliderFloat("model rx", &rx, 0.0f, 180.0f);
        slider_rotate[1] = ImGui::SliderFloat("model ry", &ry, -180.0f, 180.0f);
        slider_rotate[2] = ImGui::SliderFloat("model rz", &rz, 0.0f, 180.0f);
        if(slider_rotate[0] || slider_rotate[1] || slider_rotate[2])
            m_tranform.lock()->SetRotation(glm::vec3(rx, ry, rz));

        bool slider_scale[3];
        slider_scale[0] = ImGui::SliderFloat("model sx", &sx, 0.0f, 1.0f);
        slider_scale[1] = ImGui::SliderFloat("model sy", &sy, 0.0f, 1.0f);
        slider_scale[2] = ImGui::SliderFloat("model sz", &sz, 0.0f, 1.0f);
        if(slider_scale[0] || slider_scale[1] || slider_scale[2]){
            m_tranform.lock()->SetScale(glm::vec3(sx, sy, sz));  
        }

        if(ImGui::Button("Translate Forward")){
            m_tranform.lock()->Translate(m_tranform.lock()->Forward());
        }ImGui::SameLine();
        if(ImGui::Button("Translate Up")){
            m_tranform.lock()->Translate(m_tranform.lock()->Up());
        }ImGui::SameLine();
        if(ImGui::Button("Translate Right")){
            m_tranform.lock()->Translate(m_tranform.lock()->Right());
        }
        {
            auto p = m_tranform.lock()->LocalPosition();
            auto r = m_tranform.lock()->LocalEulerAngles();
            ImGui::Text("local pos     (%f, %f, %f)", p.x, p.y, p.z);
            ImGui::Text("local rotate  (%f, %f, %f)", r.x, r.y, r.z);
        }
        {
            auto p = m_tranform.lock()->Position();
            auto r = m_tranform.lock()->EulerAngles();
            auto s = m_tranform.lock()->Scale();
            ImGui::Text("global pos    (%f, %f, %f)", p.x, p.y, p.z);
            ImGui::Text("global rotate (%f, %f, %f)", r.x, r.y, r.z);
            ImGui::Text("       scale  (%f, %f, %f)", s.x, s.y, s.z);
        }
        
        ImGui::End();
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
    auto  grid = std::make_shared<GObj>();

    auto  cube  = std::make_shared<GObj>();
    auto  cube2 = std::make_shared<GObj>();
    
    auto  model = std::make_shared<GObj>();
    auto  model2 = std::make_shared<GObj>();
    auto  model3 = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<CameraScripts>(glm::vec3(0, 10, 15)));
    // camera->AddCom(std::make_shared<GSkybox>(
    //     std::vector<std::string>({
    //         "/home/lkp/projs/gfy/build/skybox/right.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/left.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/top.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/bottom.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/front.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/back.jpg",
    //         // "/home/lkp/projs/gfy/build/skybox3/perdicus_rt.tga",
    //         // "/home/lkp/projs/gfy/build/skybox3/perdicus_lf.tga",
    //         // "/home/lkp/projs/gfy/build/skybox3/perdicus_up.tga",
    //         // "/home/lkp/projs/gfy/build/skybox3/perdicus_dn.tga",
    //         // "/home/lkp/projs/gfy/build/skybox3/perdicus_bk.tga",
    //         // "/home/lkp/projs/gfy/build/skybox3/perdicus_ft.tga",
    //     })
    // ));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    cube->AddDefaultComs();
    // cube->AddCom(std::make_shared<GCube>());
    cube->AddCom(std::make_shared<ModelScripts>("cube", glm::vec3(0, 10, 0)));

    cube2->AddDefaultComs();
    // cube2->AddCom(std::make_shared<GCube>());
    cube2->AddCom(std::make_shared<ModelScripts>("cube2", glm::vec3(-10, 10, 0)));

    // model->AddDefaultComs();
    // model->AddCom(std::make_shared<ModelScripts>("Model1", glm::vec3(-10, 0, -10)));
    // model->AddCom(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj"));

    // model2->AddDefaultComs();
    // model2->AddCom(std::make_shared<ModelScripts>("Model2", glm::vec3(0, 0, 0)));
    // model2->AddCom(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj"));

    // model3->AddDefaultComs();
    // model3->AddCom(std::make_shared<ModelScripts>("Model3", glm::vec3(10, 0, -10)));
    // model3->AddCom(std::make_shared<GModel>("/home/lkp/projs/gfy/build/nanosuit/nanosuit.obj"));

 
    
    s->AddChild(camera);
    s->AddChild(grid);
    s->AddChild(cube);
    cube->AddChild(cube2);
    // s->AddChild(model);
    // s->AddChild(model2);
    // s->AddChild(model3);
}

int main(int argc, char** argv)
{
    std::shared_ptr<GWorld> w = std::make_shared<GWorld>();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    w->SetScene(s);
    
    build_scene(s);

    return w->Run();
}
