#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"

class Transform : public GCom
{
public:
    Transform(std::string name):
        m_name(name)
    {
    }

    virtual ~Transform()
    {
        std::cout << "de construct:" << m_name << std::endl;
    }

    virtual std::string ComName() override { return "Transform"; }

    virtual void Update() override
    {
        //std::cout << m_name << std::endl;
    }
    std::string m_name;
};

void build_scene(std::shared_ptr<GScene> s)
{
    std::shared_ptr<GObj>  g1 = std::make_shared<GObj>();
    std::shared_ptr<GObj>  g2 = std::make_shared<GObj>();
    std::shared_ptr<GObj>  g3 = std::make_shared<GObj>();

    g1->AddCom(std::make_shared<Transform>("g1"));
    g2->AddCom(std::make_shared<Transform>("g2"));
    g3->AddCom(std::make_shared<Transform>("g3"));

    g1->AddChild(g2);
    s->AddChild(g1);
    s->AddChild(g3);

    GObj::Destroy(g1);
}

int main(int argc, char** argv)
{
    GWorld::StaticInit();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    GWorld::s_instance->SetScene(s);

    build_scene(s);

    return GWorld::s_instance->Run();
}
