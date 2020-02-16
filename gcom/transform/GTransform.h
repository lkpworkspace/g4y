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

    virtual void Start() override;

    virtual void Update() override;

    void SetPostion(float x, float y, float z);
    void SetPostion(glm::vec3& vec);

    virtual std::string ComName() override { return "GTransform"; }

    /* local postion */
    glm::vec3 postion;
    glm::vec3 rotate;
    glm::vec3 scale;

    /* world postion */
    glm::vec3 wld_postion;
    glm::vec3 wld_rotate;
    glm::vec3 wld_scale;

    glm::mat4 model;

    std::weak_ptr<GTransform> m_parent_trans;
};

#endif

