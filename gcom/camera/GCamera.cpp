#include "GCamera.h"
#include "GObj.h"

GCamera::GCamera() :
    orthographic(false),
    fov(60.0f),
    aspect(1.0f),
    near(0.1f),
    far(1000.0f),
    x(0.0f),
    y(0.0f),
    w(1.0f),
    h(1.0f)
{}

GCamera::~GCamera()
{}

void GCamera::Init()
{
    m_transform = Obj()->Transform();
}

glm::mat4 GCamera::Projection()
{
    if(!orthographic)
        return glm::perspective(glm::radians(fov), aspect, near, far);
    else
        return glm::ortho(x, w, y, h, near, far);
}

void GCamera::SetCameraType(CAMERRA_TYPE t)
{
    orthographic = (t == ORTHO) ? true : false;
}

glm::mat4 GCamera::View()
{
    glm::mat4 translate(1.0f);
    
    translate = glm::translate(translate, m_transform.lock()->Position() * -1.0f);
    
    auto q = m_transform.lock()->Rotation();

    return glm::mat4_cast(glm::inverse(q)) * translate;
}