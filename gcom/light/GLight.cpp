#include "GLight.h"
#include "GShader.h"
#include "GObj.h"
#include "GCamera.h"

GLight::GLight()
{}

GLight::~GLight()
{}


void GLight::Start()
{    
    m_transform = Obj()->Transform();
    m_camera = std::static_pointer_cast<GCamera>(Obj()->FindWithTag("GCamera")->GetCom("GCamera"));
}

void GLight::OnRender()
{
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 P = m_camera.lock()->Projection();
    glm::mat4 V = m_camera.lock()->View();
    glm::mat4 M = m_transform.lock()->model;
}

void GLight::Exit()
{
    
}