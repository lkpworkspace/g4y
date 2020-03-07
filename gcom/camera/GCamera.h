#ifndef __GCAMERA_H__
#define __GCAMERA_H__
#include "GCom.h"

class GTransform;
class GCamera : public GCom
{
public:
    GCamera();
    virtual ~GCamera();

    virtual void Init() override;

    virtual std::string ComName() override { return "GCamera"; }

    glm::mat4 Projection();
    glm::mat4 View();
private:
    // 摄像机的指向向量 = glm::normalize(position - target);

    float fov;
    float aspect;
    float near;
    float far;

    std::weak_ptr<GTransform> m_transform;
};

#endif