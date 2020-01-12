#ifndef __GCOM_H__
#define __GCOM_H__
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifdef USE_GUI
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

class GObj;
class GCom
{
    friend class GObj;
public:
    GCom();
    virtual ~GCom();

    virtual void Init(){}

    virtual void Awake(){}

    virtual void Update(){}

    virtual void LateUpdate() {}

    virtual void OnRender(){}

    virtual void OnGUI(){}

    virtual void Exit(){}

    std::shared_ptr<GObj> Obj() { return m_obj.lock(); }

    virtual std::string ComName() { return "GCom"; }

protected:
    void OnAwake();
private:
    bool                m_awake;
    std::weak_ptr<GObj> m_obj;
};

#endif