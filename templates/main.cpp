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
		// ����������ת
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

	// ���һ��Transform���
	camera->AddDefaultComs();
	// ���������
	camera->AddCom(std::make_shared<GCamera>());
	// �������������
	camera->AddCom(std::make_shared<RoamScript>());

	// ���һ��Transform���
	grid->AddDefaultComs();
	// ���һ���������
	grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));
	// ���һ������ذ�
	grid->AddCom(std::make_shared<GMeshCollider>());

	// ���һ��Transform���
	sample_obj->AddDefaultComs();
	// ���һ��������
	sample_obj->AddCom(std::make_shared<GAxis>());
	// ���һ��cube
	sample_obj->AddCom(std::make_shared<GCube>());
	// ���һ����������ײ��
	//auto box_collider = std::make_shared<GBoxCollider>();
	//sample_obj->AddCom(box_collider);
	// ���һ������
	//sample_obj->AddCom(std::make_shared<GRigibody>(box_collider));
	// ���һ������������ת���
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
