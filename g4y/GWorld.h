#ifndef __GWORLD_H__
#define __GWORLD_H__
#include <unordered_set>
#include <memory>
#ifdef USE_GUI
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

class GScene;
class GDynamicsWorld;
class GWorld : public std::enable_shared_from_this<GWorld>
{
    friend class GScene;
public:
    GWorld();
    virtual ~GWorld();

    void Update();
    void SetScene(std::shared_ptr<GScene> s);

#ifdef USE_GUI
    GLFWwindow* window;
#endif
private:
    int  InitGL();
    void ExitGL();
    std::shared_ptr<GScene> m_scene;
    std::shared_ptr<GDynamicsWorld> m_phy_world;
};

#endif
