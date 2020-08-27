#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>

#include "G4Y.h"
#include "../examples/RoamScript.h"

class TestCom : public GCom
{
    G_COM
public:
	virtual void Start() override
	{
		m_transform = GetCom<GTransform>();
		m_transform.lock()->SetPosition(glm::vec3(0,5,0));
	}

    virtual void Update() override
    {
		// 控制物体自转
		m_yaw += GWorld::GetDeltaTime() * 90;
		m_transform.lock()->SetRotation(glm::vec3(0, m_yaw, 0));
	}

private:
	float m_yaw = 0.0f;
	std::weak_ptr<GTransform> m_transform;
};

void build_scene(std::shared_ptr<GScene> s)
{
	auto camera = std::make_shared<GObj>();
	auto grid = std::make_shared<GObj>();
	auto sample_obj = std::make_shared<GObj>();

	// 添加一个Transform组件
	camera->AddDefaultComs();
	// 添加相机组件
	camera->AddCom(std::make_shared<GCamera>());
	// 添加相机漫游组件
	camera->AddCom(std::make_shared<RoamScript>());

	// 添加一个Transform组件
	grid->AddDefaultComs();
	// 添加一个网格组件
	grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));
	// 添加一个物理地板
	grid->AddCom(std::make_shared<GMeshCollider>());

	// 添加一个Transform组件
	sample_obj->AddDefaultComs();
	// 添加一个坐标轴
	sample_obj->AddCom(std::make_shared<GAxis>());
	// 添加一个cube
	sample_obj->AddCom(std::make_shared<GCube>());
	// 添加一个立方体碰撞框
	//auto box_collider = std::make_shared<GBoxCollider>();
	//sample_obj->AddCom(box_collider);
	// 添加一个刚体
	//sample_obj->AddCom(std::make_shared<GRigibody>(box_collider));
	// 添加一个控制物体自转组件
	sample_obj->AddCom(std::make_shared<TestCom>());

	s->AddChild(camera);
	s->AddChild(grid);
	s->AddChild(sample_obj);
}

// SDL defines main
#undef main
int main(int argc, char** argv)
{
    const auto w = GWorld::Instance();
	w->Init(argc, argv);

    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    w->SetScene(s);

    build_scene(s);

    return w->Run();
}
