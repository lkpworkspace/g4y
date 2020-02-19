#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "CameraScripts.h"

class ColliderScripts : public GCom
{
public:
    ColliderScripts(std::string title) :
        title(title)
    {}

    virtual void Start() override
    {
        m_tranform = Obj()->Transform();
        m_dwld = Obj()->Scene()->PhyWorld();
        m_rigibody = Obj()->GetCom<GRigibody>("GRigibody");
    }

    virtual void Update() override
    {
         ImGui::Begin(title.c_str());

        if(ImGui::Button("SetY")){
            
            btTransform trans(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0));
            m_rigibody.lock()->m_rigidbody->setWorldTransform(trans);
        }

        if(ImGui::Button("Move +Y")){            
            m_rigibody.lock()->m_rigidbody->translate(btVector3(0, 1, 0));
        }ImGui::SameLine();
        if(ImGui::Button("Move -Y")){            
            m_rigibody.lock()->m_rigidbody->translate(btVector3(0, -1, 0));
        }ImGui::SameLine();
        if(ImGui::Button("Move +Z")){            
            m_rigibody.lock()->m_rigidbody->translate(btVector3(0, 0, 2));
        }ImGui::SameLine();
        if(ImGui::Button("Move -Z")){            
            m_rigibody.lock()->m_rigidbody->translate(btVector3(0, 0, -2));
        }ImGui::SameLine();
        if(ImGui::Button("Move +X")){            
            m_rigibody.lock()->m_rigidbody->translate(btVector3(2, 0, 0));
        }ImGui::SameLine();
        if(ImGui::Button("Move -X")){            
            m_rigibody.lock()->m_rigidbody->translate(btVector3(-2, 0, 0));
        }
        auto rigibody = m_rigibody.lock()->m_rigidbody;
        ImGui::Text("CollisionFlags              (%d)", rigibody->getCollisionFlags());
        ImGui::Text("ActivationState             (%d)", rigibody->getActivationState());
        ImGui::Text("ContactResponse             (%d)", rigibody->hasContactResponse());
        ImGui::Text("static %d, kinematic %d, static or kinematic %d, ", rigibody->isStaticObject(), rigibody->isKinematicObject(), rigibody->isStaticOrKinematicObject());
        ImGui::Text("AnisotropicFriction         (%d, %f, %f, %f)", rigibody->hasAnisotropicFriction(), rigibody->getAnisotropicFriction().getX(), rigibody->getAnisotropicFriction().getY(), rigibody->getAnisotropicFriction().getZ());

        ImGui::End();
    }

    virtual void OnCollisionEnter() override
    {
        std::cout << "[OnCollisionEnter] " << Obj()->UUID() << std::endl;
    }

    virtual void OnCollisionStay() override
    {
        // std::cout << "[OnCollisionStay] " << Obj()->UUID() << std::endl;
    }

    virtual void OnCollisionExit() override
    {
        std::cout << "[OnCollisionExit] " << Obj()->UUID() << std::endl;
    }

    std::string title;
    std::weak_ptr<GTransform> m_tranform;
    std::weak_ptr<GRigibody>  m_rigibody;
    std::weak_ptr<GPhyWorld>  m_dwld;
};

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  grid   = std::make_shared<GObj>();
    auto  ground = std::make_shared<GObj>();
    auto  sphere = std::make_shared<GObj>();
    auto  sphere2 = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<CameraScripts>());

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    ground->AddDefaultComs();
    ground->AddCom(std::make_shared<GMeshCollider>());

    sphere->AddDefaultComs();
    sphere->Transform()->SetPostion(-5, 10, 0);
    sphere->AddCom(std::make_shared<GRigibody>());
    sphere->AddCom(std::make_shared<GSphereCollider>());
    sphere->AddCom(std::make_shared<ColliderScripts>("set sphere"));

    sphere2->AddDefaultComs();
    sphere2->Transform()->SetPostion(5, 10, 0);
    sphere2->AddCom(std::make_shared<GRigibody>());
    sphere2->AddCom(std::make_shared<GSphereCollider>());
    sphere2->AddCom(std::make_shared<ColliderScripts>("set sphere2"));

    s->AddChild(camera);
    s->AddChild(grid);
    s->AddChild(ground);
    s->AddChild(sphere);
    s->AddChild(sphere2);
}

int main(int argc, char** argv)
{
    std::shared_ptr<GWorld> w = std::make_shared<GWorld>();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();
    
    w->SetScene(s);
    
    build_scene(s);

    return w->Run();
}
