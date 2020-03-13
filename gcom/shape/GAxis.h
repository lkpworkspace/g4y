#ifndef __GAXIS_H__
#define __GAXIS_H__
#include "GCom.h"

class GShader;
class GCamera;
class GTransform;
class GAxis : public GCom
{
public:
    GAxis(){}
    virtual ~GAxis(){}

    virtual void Start() override;

    virtual void OnRender() override;

    virtual void OnDestroy() override;

private:

    // VAO: 顶点数组对象
    // VBO: 顶点缓冲对象
    //  - 它会在GPU内存（通常被称为显存）中储存大量顶点
    //  - GL_ARRAY_BUFFER
    // EBO: 索引缓冲对象
    unsigned int VAO, VBO;
    std::shared_ptr<GShader> m_shader;
    std::weak_ptr<GCamera>   m_camera;
    std::weak_ptr<GTransform>m_transform;
};

#endif