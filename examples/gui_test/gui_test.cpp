#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "RoamScript.h"
#include "CameraScript.h"
#include "BulletScript.h"
#include "MoveScript.h"

void create_bullet(std::shared_ptr<GScene> s, glm::vec3 pos, glm::quat rot, float speed)
{
    auto o = std::make_shared<GObj>();

    o->AddDefaultComs();
    o->AddCom(std::make_shared<BulletScript>(pos, rot, speed));
    s->AddChild(o);
}

class RotateScript : public GCom
{
public:
    RotateScript(std::string title) :
        title(title)
    {}

    virtual void Start() override
    {
        m_tranform = Obj()->Transform();
        m_tranform.lock()->SetPosition(glm::vec3(0, 0, 0));
    }

    virtual void Update() override
    {
        float delta_time = GWorld::GetDeltaTime();
        float speed = 180.0f;
        glm::vec3 target = glm::vec3(1,0,0);
        //if(g4y::GetKey(0x72))
        {
            rotate_angle = delta_time * speed;
            m_tranform.lock()->RotateAround(target, glm::vec3(0,1,0), rotate_angle);
            m_tranform.lock()->LookAt(target);
        }
        static float time = 0.0f;
        time += delta_time;
        if(time > 0.5f){
            create_bullet(Obj()->Scene(), m_tranform.lock()->Position(), m_tranform.lock()->Rotation(), 8.0f);
            time = 0.0f;
        }
    }

    float rotate_angle = 0.0f;
    std::string title;
    std::weak_ptr<GTransform> m_tranform;
};

template<typename T>
std::string GType()
{
    return typeid(T).name();
}

void build_scene(std::shared_ptr<GScene> s)
{
    double t = g4y::gettime();
    g4y::resourcemgr()->SetResourceDir("/home/lkp/projs/g4y/asset/");
    g4y::resourcemgr()->LoadModel("nanosuit/nanosuit.obj");
    g4y::resourcemgr()->LoadModel("MC4/MCblocks.obj");
    std::cout << "load model : " << g4y::gettime() - t << std::endl;

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
    camera->AddCom(std::make_shared<RoamScript>("Camera roam"));
    camera->AddCom(std::make_shared<GSkybox>(
        std::vector<std::string>({
            "/home/lkp/projs/gfy/build/skybox/right.jpg",
            "/home/lkp/projs/gfy/build/skybox/left.jpg",
            "/home/lkp/projs/gfy/build/skybox/top.jpg",
            "/home/lkp/projs/gfy/build/skybox/bottom.jpg",
            "/home/lkp/projs/gfy/build/skybox/front.jpg",
            "/home/lkp/projs/gfy/build/skybox/back.jpg",
        })
    ));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    cube->AddDefaultComs();
    // cube->AddCom(std::make_shared<GCube>());
    //cube->AddCom(std::make_shared<ModelScripts>("cube", glm::vec3(0, 10, 0)));

    cube2->AddDefaultComs();
    // cube2->AddCom(std::make_shared<GCube>());
    //cube2->AddCom(std::make_shared<ModelScripts>("cube2", glm::vec3(-10, 10, 0)));
    cube2->AddCom(std::make_shared<RotateScript>("rotate"));

    // model->AddDefaultComs();
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
    s->AddChild(cube2);
    for(int i = 0; i < 10; ++i){
        // auto obj = g4y::resourcemgr()->Instantiate("MC2/MCblocks.obj");
        //polySurface820
        std::string node_name = "polySurface";
        node_name += std::to_string(i + 800);
        auto obj = g4y::resourcemgr()->CloneChildNode("MC4/MCblocks.obj", node_name);
        if(obj == nullptr){
            std::cout << "find null" << std::endl;
            break;
        }
        obj->AddCom(std::make_shared<MoveScript>(std::string("obj") + std::to_string(i)));
        s->AddChild(obj);
    }
    //cube->AddChild(camera);
    // cube->AddChild(cube2);
    // s->AddChild(model);
    // s->AddChild(model2);
    // s->AddChild(model3);
}

int main(int argc, char** argv)
{
    const auto w = GWorld::Instance();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    w->SetScene(s);

    build_scene(s);

    return w->Run();
}
