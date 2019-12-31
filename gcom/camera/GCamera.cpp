#include "GCamera.h"


GCamera::GCamera() :
    position(0.0f, 0.0f, 10.0f),
    target(0.0f, 0.0f, 0.0f),
    up(0.0f, 1.0f, 0.0f),
    fov(60.0f),
    aspect(1.0f),
    near(0.1f),
    far(100.0f)
{}

GCamera::~GCamera()
{}

glm::mat4 GCamera::Projection()
{
    return glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::mat4 GCamera::View()
{
    return glm::lookAt(position, target, up);
}