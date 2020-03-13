#ifndef __GLIGHT_H__
#define __GLIGHT_H__
#include "GCom.h"

class GShader;
class GCamera;
class GTransform;
class GLight : public GCom
{
public:
    GLight();
    virtual ~GLight();

    virtual void Start() override;

    virtual void OnRender() override;

    virtual void OnDestroy() override;

    /* 环境光 */
    glm::vec3 ambient;
    /* 漫反射 */
    glm::vec3 diffuse;
    /* 镜面反射 */
    glm::vec3 specular;

    std::weak_ptr<GCamera>   m_camera;
    std::weak_ptr<GTransform>m_transform;
};

class GDirLight : public GLight
{
public:
    GDirLight(){}
    virtual ~GDirLight(){}

    /* 平行光方向 */
    glm::vec3 direction;
};

class GPointLight : public GLight
{
public:
    GPointLight(){}
    virtual ~GPointLight(){}

    /* 常数项 */
    float constant;
    /* 一次项 */
    float linear;
    /* 二次项 */
    float quadratic;
};

class GSpotLight : public GLight
{
public:
    GSpotLight(){}
    virtual ~GSpotLight(){}

    /* 平行光方向 */
    glm::vec3 m_direction;

    float cut_off;
    float outer_cut_off;
  
     /* 常数项 */
    float constant;
    /* 一次项 */
    float linear;
    /* 二次项 */
    float quadratic;
};
#endif