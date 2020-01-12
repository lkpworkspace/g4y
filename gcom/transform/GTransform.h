#ifndef __GTRANSFORM_H__
#define __GTRANSFORM_H__
#include "GCom.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <iostream>
class GTransform : public GCom
{
public:
    GTransform();
    virtual ~GTransform();

    virtual void Update() override;

    virtual std::string ComName() override { return "GTransform"; }

    glm::vec3 postion;
    glm::vec3 rotate;
    glm::vec3 scale;
    glm::mat4 model;
};

#endif

