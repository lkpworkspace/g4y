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
        PUT_UP,       // +y 1
        PUT_DOWN,     // -y 2
        PUT_LEFT,     // -x 3
        PUT_RIGHT,    // +x 4
        PUT_FRONT,    // +z 5
        PUT_BACK,     // -z 6
    };
public:
    virtual void Start() override
    {
        m_tranform = GetCom<GTransform>();
        m_camera = Obj()->FindWithTag("GCamera");
    }

    virtual void Update() override
    {
		
        if(ImGui::IsMouseClicked(0)){
			 
			GRayHit hit;
			auto from = m_camera.lock()->GetCom<GTransform>()->Position();
			auto to = RayTo(m_camera.lock(), ImGui::GetMousePos().x, ImGui::GetMousePos().y);

			if(g4y::phyworld()->RayTest(from, to, hit)){
				std::cout << hit.obj.lock()->Tag() << std::endl;
				if(hit.obj.lock()->Tag() == "Block"){
					PUTPOS type = PUT_NONE;
					auto bpos = hit.obj.lock()->GetCom<GTransform>()->Position();

					printf("click obj pos %f, %f, %f\n", bpos.x, bpos.y, bpos.z);
					printf("hit       pos %f, %f, %f\n", hit.pick_pos.x, hit.pick_pos.y, hit.pick_pos.z);
					type = GetPutPos(bpos, hit.pick_pos);
					for(int i = 0; i < 6; ++i){
						if(type == ary[i].first){
							auto t = bpos + ary[i].second * 2.0f;
							printf("get target pos %f %f, %f, ret %f %f, %f\n", bpos.x, bpos.y, bpos.z, t.x, t.y, t.z);
							auto cube = CreateCube(t);
							g4y::curscene()->AddChild(cube);
						}
					}
					std::cout << "put type " << (int)type << std::endl << std::endl;
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

    enum PUTPOS GetPutPos(glm::vec3 objpos, glm::vec3 hitpos)
    {
		auto v = hitpos - objpos;
        for(int i = 0; i < ary.size(); ++i){
			for (int j = 0; j < 3; ++j) {
				if (fabs(v[j] - ary[i].second[j]) <= 1e-3) return ary[i].first;
			}
        }
        return PUT_NONE;
    }

    glm::vec3 RayTo(std::weak_ptr<GObj> o/* camera */, float x, float y)
    {
        int w, h;
        auto camera = o.lock()->GetCom<GCamera>();
        auto camera_trans = o.lock()->GetCom<GTransform>();

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
        rayto += x * dhor;
        rayto -= y * dvert;
        return rayto;
    }

    std::shared_ptr<GObj> CreateCube(glm::vec3 pos)
    {
        //auto obj = std::make_shared<GObj>();
		std::string model_name = "MC/" + std::to_string(obj_i + 1) + ".obj";
		auto obj = g4y::resourcemgr()->Instantiate(model_name);
		obj_i = obj_i++ % 36;
    
        obj->GetCom<GTransform>()->SetPosition(pos);
        
        //auto cube = std::make_shared<GCube>();
        ////cube->SetColor(glm::vec4());
        //obj->AddCom(cube);

        auto collider = std::make_shared<GBoxCollider>();
        obj->AddCom(collider);
        // obj->AddCom(std::make_shared<GRigibody>());
        
        auto block = std::make_shared<Block>();
        obj->AddCom(block);
        return obj;
    }

	int obj_i = 0;

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