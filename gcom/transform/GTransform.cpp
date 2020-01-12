#include "GTransform.h"

GTransform::GTransform() :
    postion(0, 0, 0),
    rotate(0, 0, 0),
    scale(1, 1, 1)
{
    model = glm::mat4(1.0f);
}

GTransform::~GTransform()
{}

void GTransform::Update()
{
    glm::mat4 t(1.0f);
    glm::mat4 r(1.0f);
    glm::mat4 s(1.0f);

    t = glm::translate(t, postion);

    r = glm::rotate(r, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    r = glm::rotate(r, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    r = glm::rotate(r, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    s = glm::scale(s, scale);

    //model = s * r * t;
    model = t * s * r;
}