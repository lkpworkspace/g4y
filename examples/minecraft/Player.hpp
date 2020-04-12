#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include <array>
#include <cmath>
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#include "G4Y.h"

class Block;
/*
主要功能：
    铺设地砖
    场景漫游
*/
class Player : public GCom
{
    enum PUTPOS{
        PUT_NONE,
        PUT_UP,       // +y
        PUT_DOWN,     // -y
        PUT_LEFT,     // -x
        PUT_RIGHT,    // +x
        PUT_FRONT,    // +z
        PUT_BACK,     // -z
    };
public:
    virtual void Start() override
    {
        m_tranform = Obj()->Transform();
        m_camera = Obj()->FindWithTag("GCamera");
    }

    virtual void Update() override
    {
         if(ImGui::IsMouseClicked(0)){
            auto pos = RayTo(m_camera.lock(), ImGui::GetMousePos().x, ImGui::GetMousePos().y);
            GRayHit hit;
            if(g4y::phyworld()->RayTest(m_camera.lock()->Transform()->Position(), pos, hit)){
                std::cout << hit.obj.lock()->Tag() << std::endl;
                if(hit.obj.lock()->Tag() == "Block"){
                    auto bpos = hit.obj.lock()->Transform()->Position();
                    auto type = GetPutPos(bpos, hit.pick_pos);
                    for(int i = 0; i < 6; ++i){
                        if(type == ary[i].first){
                            auto t = bpos + ary[i].second * 2.0f;
                            printf("get target pos %f %f, %f, ret %f %f, %f\n", bpos.x, bpos.y, bpos.z, t.x, t.y, t.z);
                            auto cube = CreateCube(t);
                            g4y::curscene()->AddChild(cube);
                        }
                    }
                    std::cout << "put type " << (int)type << std::endl;
                }
            }
        }

        ImGui::Begin("Player");

        if(ImGui::Button("Create cube")){
            auto cube = CreateCube(glm::vec3(0, 0, 0));
            g4y::curscene()->AddChild(cube);
        }

        ImGui::End(); 
    }

    // TODO 第二个计算错误。。。
    enum PUTPOS GetPutPos(glm::vec3 objpos, glm::vec3 hitpos)
    {
        for(int i = 0; i < ary.size(); ++i){
            auto v = glm::normalize(hitpos - objpos);
            auto s = glm::normalize(ary[i].second);
            auto r = glm::dot(v, s);
            //printf("r %f,  v %f, %f, %f,  s %f, %f, %f\n", r, v.x, v.y, v.z, s.x, s.y, s.z);
            if(r <= 1 && r >= (M_PI / 4)){
                return ary[i].first;
            }
        }
        return PUT_NONE;
    }

    glm::vec3 RayTo(std::weak_ptr<GObj> o/* camera */, int x, int y)
    {
        int w, h;
        auto camera = o.lock()->GetCom<GCamera>("GCamera");
        auto camera_trans = o.lock()->Transform();

        g4y::glview()->GetWindowSize(w, h);

        auto ray_from = camera_trans->Position();
        auto ray_forward = camera_trans->Forward() * -1.0f;
        auto near = camera->NearFar().x;
        auto far  = camera->NearFar().y;
        ray_forward *= camera->NearFar().y;

        auto hor      = camera_trans->Right();
        auto vertical = camera_trans->Up();
        auto aspect   = camera->FovAspect().y;
        auto fov      = camera->FovAspect().x;

        auto tan_fov  = tanf(glm::radians(.5f * fov));
        hor           *= 2.f * far * tan_fov;
        vertical      *= 2.f * far * tan_fov;

        hor *= aspect;

        auto ray2center = ray_from + ray_forward;
        auto dhor = hor * 1.f / (float)w;
        auto dvert= vertical * 1.0f / (float)h;

        auto rayto = ray2center - 0.5f * hor + 0.5f * vertical;
        rayto += float(x) * dhor;
        rayto -= float(y) * dvert;
        return rayto;
    }

    std::shared_ptr<GObj> CreateCube(glm::vec3 pos)
    {
        auto obj = std::make_shared<GObj>();
    
        obj->AddDefaultComs();
        obj->Transform()->SetPosition(pos);
        
        auto cube = std::make_shared<GCube>();
        //cube->SetColor(glm::vec4());
        obj->AddCom(cube);

        auto collider = std::make_shared<GBoxCollider>();
        // obj->AddCom(std::make_shared<GRigibody>());
        obj->AddCom(collider);
        
        auto block = std::make_shared<Block>();
        obj->AddCom(block);
        return obj;
    }

    std::array<std::pair<enum PUTPOS, glm::vec3>, 6> ary{
            std::make_pair(PUT_UP,    glm::vec3(0, 0.5f, 0)),
            std::make_pair(PUT_DOWN,  glm::vec3(0, -.5f, 0)),
            std::make_pair(PUT_LEFT,  glm::vec3(-.5f, 0, 0)),
            std::make_pair(PUT_RIGHT, glm::vec3(0.5f, 0, 0)),
            std::make_pair(PUT_FRONT, glm::vec3(0, 0, 0.5f)),
            std::make_pair(PUT_BACK,  glm::vec3(0, 0, -.5f))
    };

    std::weak_ptr<GTransform> m_tranform;
    std::weak_ptr<GObj> m_camera;
};

#endif