#ifndef __GOPENGLVIEW_H__
#define __GOPENGLVIEW_H__
#include <memory>
#ifdef USE_GRAPHICS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

class GShader;
class GOpenGLView
{
public:
    int InitGL();

    std::shared_ptr<GShader> GetShader() { return m_global_shader; }

    bool WindowShouldClose();

    void SetCursorPos(double x, double y);

    void ShowDemo(bool);

    void GetWindowSize(int& w, int& h);

    void SetRenderRect(glm::ivec4);

    void BeginRender();

    void PollEvents();

    void EndRender();

    void ExitGL();

private:
#ifdef USE_GRAPHICS
    void InitShader();

    std::shared_ptr<GShader> m_global_shader;
    GLFWwindow* window;
#endif
    bool m_show_demo = true;
};

#endif