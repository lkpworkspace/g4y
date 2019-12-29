#include <iostream>
#include <string>

#include "GObj.h"
#include "GCom.h"
#include "GScene.h"
#include "GWorld.h"

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

    virtual std::string GetComName() override { return "Transform"; }
    virtual void Update() override
    {
        std::cout << m_name << std::endl;
    }
    std::string m_name;
};

void build_world(std::shared_ptr<GScene> s)
{
    std::shared_ptr<GObj>  g1 = std::make_shared<GObj>();
    std::shared_ptr<GObj>  g2 = std::make_shared<GObj>();
    std::shared_ptr<GObj>  g3 = std::make_shared<GObj>();

    g1->AddCom(std::static_pointer_cast<GCom>(std::make_shared<Transform>("g1")));
    g2->AddCom(std::static_pointer_cast<GCom>(std::make_shared<Transform>("g2")));
    g3->AddCom(std::static_pointer_cast<GCom>(std::make_shared<Transform>("g3")));

    g1->AddChild(g2);
    s->AddChild(g1);
    s->AddChild(g3);

    s->DelChild(g1);
}

int main(int argc, char** argv)
{
    std::shared_ptr<GScene> s = std::make_shared<GScene>();
    s->SetCurScene();
    build_world(s);

    char buf[256];
    int cnt = 0;
    while(std::cin.getline(buf, sizeof(buf))){
        std::cout << "Update : " << cnt++ << std::endl; 
        s->Update();
    }
    return 0;
}