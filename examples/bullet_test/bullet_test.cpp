#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "G4Y.h"
#include "CameraScript.h"

class ColliderScripts : public GCom
{
public:
    ColliderScripts(std::string title) :
        title(title)
    {}

    virtual void Start() override
    {
        m_tranform = Obj()->Transform();
        m_dwld = g4y::phyworld();
        m_rigibody = Obj()->GetCom<GRigibody>("GRigibody");
    }

    virtual void Update() override
    {
         ImGui::Begin(title.c_str());

         if(ImGui::Button("Set Trigger True")){
            auto rigidbody = m_rigibody.lock()->m_rigidbody;
            rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
        }ImGui::SameLine();
        if(ImGui::Button("Set Trigger false")){
            auto rigidbody = m_rigibody.lock()->m_rigidbody;
            rigidbody->setCollisionFlags(0);
        }
        if(ImGui::Button("SetAnisotropicFriction")){
            m_rigibody.lock()->m_rigidbody->setAnisotropicFriction(btVector3(2,2,2), btCollisionObject::CF_ANISOTROPIC_FRICTION);
        }ImGui::SameLine();
        if(ImGui::Button("Add Constraint")){
            //m_rigibody.lock()->m_rigidbody->setAnisotropicFriction(btVector3(2,2,2), btCollisionObject::CF_ANISOTROPIC_FRICTION);
            btVector3 pivotInA(0,5,0);
            btTypedConstraint* p2p = new btPoint2PointConstraint(*m_rigibody.lock()->m_rigidbody.get(), pivotInA);
            g4y::phyworld()->m_dynamics_world->addConstraint(p2p);
        }

        if(ImGui::Button("SetY")){
            btTransform trans(btQuaternion(0, 30, 0), btVector3(0, 10, 0));
            m_rigibody.lock()->m_rigidbody->setWorldTransform(trans);
        }ImGui::SameLine();
        if(ImGui::Button("Rotate +Y")){
            auto q = m_tranform.lock()->Rotation();
            auto p = m_tranform.lock()->Position();
            q = glm::rotate(q, glm::radians(45.f), glm::vec3(0,1,0));
            m_rigibody.lock()->m_rigidbody->setWorldTransform(btTransform{btQuaternion(q.x, q.y, q.z, q.w), btVector3(p.x, p.y, p.z)});
        }ImGui::SameLine();
        if(ImGui::Button("Rotate -Y")){
            auto q = m_tranform.lock()->Rotation();
            auto p = m_tranform.lock()->Position();
            q = glm::rotate(q, glm::radians(-45.f), glm::vec3(0,1,0));
            m_rigibody.lock()->m_rigidbody->setWorldTransform(btTransform{btQuaternion(q.x, q.y, q.z, q.w), btVector3(p.x, p.y, p.z)});
        }ImGui::SameLine();
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

        if(ImGui::Button("Move forward")){            
            auto forward = m_tranform.lock()->Forward();
            m_rigibody.lock()->m_rigidbody->translate(btVector3(forward.x, forward.y, forward.z));
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

    virtual void OnTriggerEnter()override{
        std::cout << "[OnTriggerEnter] " << Obj()->UUID() << std::endl;
    }
    virtual void OnTriggerStay()override{
        // std::cout << "[OnTriggerStay] " << Obj()->UUID() << std::endl;
    }
    virtual void OnTriggerExit()override{
        std::cout << "[OnTriggerExit] " << Obj()->UUID() << std::endl;
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
    camera->AddCom(std::make_shared<CameraScript>());

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));

    ground->AddDefaultComs();
    ground->AddCom(std::make_shared<GMeshCollider>());

    sphere->AddDefaultComs();
    sphere->Transform()->Translate(-5, 10, 0);
    sphere->AddCom(std::make_shared<GRigibody>());
    sphere->AddCom(std::make_shared<GSphereCollider>());
    sphere->AddCom(std::make_shared<ColliderScripts>("set sphere"));

    sphere2->AddDefaultComs();
    sphere2->Transform()->Translate(5, 10, 0);
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
#if 1
    GWorld::StaticInit();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    GWorld::s_instance->SetScene(s);

    build_scene(s);

    return GWorld::s_instance->Run();
#else
    glm::mat4 m(1.0f);

    //m = glm::translate(m, glm::vec3(1.0f, 0.0f, 0.0f));
    m = glm::rotate(m, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    auto v4 = m[0];
    std::cout << v4.x << std::endl;
    std::cout << v4.y << std::endl;
    std::cout << v4.z << std::endl;
    std::cout << v4.w << std::endl << std::endl;

    v4 = m[1];
    std::cout << v4.x << std::endl;
    std::cout << v4.y << std::endl;
    std::cout << v4.z << std::endl;
    std::cout << v4.w << std::endl << std::endl;

    v4 = m[2];
    std::cout << v4.x << std::endl;
    std::cout << v4.y << std::endl;
    std::cout << v4.z << std::endl;
    std::cout << v4.w << std::endl << std::endl;

    v4 = m[3];
    std::cout << v4.x << std::endl;
    std::cout << v4.y << std::endl;
    std::cout << v4.z << std::endl;
    std::cout << v4.w << std::endl << std::endl;

#endif
}
