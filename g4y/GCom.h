#ifndef __GCOM_H__
#define __GCOM_H__
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifdef USE_GRAPHICS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#endif

class GObj;
class GPhyWorld;
class GCom
{
    friend class GObj;
public:
    GCom();
    virtual ~GCom();

    /* 初始化函数,在该组件加入到GObj被调用 */
    virtual void Init(){}

    /* 在场景循环前被调用一次,随后不再别调用 */
    virtual void Start(){}

    virtual void OnCollisionEnter(){};
    virtual void OnCollisionStay(){};
    virtual void OnCollisionExit(){};

    virtual void OnTriggerEnter(){};
    virtual void OnTriggerStay(){};
    virtual void OnTriggerExit(){};

    /* 每一帧更新都会被调用 */
    virtual void Update(){}

    /* 在Update之后被调用 */
    virtual void LateUpdate() {}

    virtual void OnRender(){}

    /* 移除场景被调用 */
    virtual void OnDestroy(){}

    std::shared_ptr<GObj> Obj() { assert(!m_obj.expired()); return m_obj.lock(); }

    virtual std::string ComName() { return "GCom"; }

protected:
    void OnStart();
private:
    bool                     m_start;
    bool                     m_destroy;
    std::weak_ptr<GObj>      m_obj;
};

#endif