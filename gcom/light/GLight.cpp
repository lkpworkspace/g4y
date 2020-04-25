#include "GLight.h"
#include "GShader.hpp"
#include "GObj.h"
#include "GCamera.h"
#include "GTransform.h"

GLight::GLight()
{}

GLight::~GLight()
{}


void GLight::Start()
{    
    m_transform = GetCom<GTransform>();
    m_camera = Obj()->FindWithTag("GCamera")->GetCom<GCamera>();
}

void GLight::OnRender()
{
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 P = m_camera.lock()->Projection();
    glm::mat4 V = m_camera.lock()->View();
    glm::mat4 M = m_transform.lock()->ToMat4();
}

void GLight::OnDestroy()
{
    
}