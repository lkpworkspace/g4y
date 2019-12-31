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