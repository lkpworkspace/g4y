#include "GCamera.h"
#include "GObj.h"

GCamera::GCamera() :
    position(0.0f, 0.0f, 10.0f),
    target(0.0f, 0.0f, 0.0f),
    up(0.0f, 1.0f, 0.0f),
    fov(60.0f),
    aspect(1.0f),
    near(0.1f),
    far(1000.0f)
{}

GCamera::~GCamera()
{}

void GCamera::Init()
{
    m_transform = Obj()->Transform();
}

glm::mat4 GCamera::Projection()
{
    return glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::mat4 GCamera::View()
{
    glm::mat4 translate(1.0f);
    glm::mat4 ratote(1.0f);
    glm::mat4 scale(1.0f);
    
    translate = glm::translate(translate, m_transform.lock()->postion * glm::vec3(-1,-1,-1));
    
    glm::vec3 r = m_transform.lock()->rotate;
    ratote = glm::rotate(ratote, glm::radians(-r.x), glm::vec3(1.0f, 0.0f, 0.0f));
    ratote = glm::rotate(ratote, glm::radians(-r.y), glm::vec3(0.0f, 1.0f, 0.0f));
    ratote = glm::rotate(ratote, glm::radians(-r.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    scale = glm::scale(scale, m_transform.lock()->scale);

    return scale * ratote * translate;
}