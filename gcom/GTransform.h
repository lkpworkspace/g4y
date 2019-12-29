#ifndef __GTRANSFORM_H__
#define __GTRANSFORM_H__
#include "GCom.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class GTransform : public GCom
{
public:
    GTransform(){}
    virtual ~GTransform(){}

    virtual std::string GetComName() override { return "GTransform"; }
    virtual void Update() override
    {

    }

    glm::vec3 postion;

private:
    glm::mat4x4 m_model;
};

#endif

