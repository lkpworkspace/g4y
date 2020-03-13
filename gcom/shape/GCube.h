#ifndef __GCUBE_H__
#define __GCUBE_H__
#include "GCom.h"

class GShader;
class GCamera;
class GTransform;
class GCube : public GCom
{
public:
    GCube();
    virtual ~GCube();

    virtual void Start() override;

    virtual void OnRender() override;

    virtual void OnDestroy() override;

private:

    unsigned int VAO, VBO;
    std::shared_ptr<GShader> m_shader;
    std::weak_ptr<GCamera>   m_camera;
    std::weak_ptr<GTransform>m_transform;
};

#endif