#ifndef __GOPENGLVIEW_H__
#define __GOPENGLVIEW_H__
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class GShader;
class GOpenGLView
{
public:
    int Init(const boost::property_tree::ptree& cfg);

	std::shared_ptr<GShader> GetShader();

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
    void InitShader();

    std::shared_ptr<GShader> m_main_shader;
    GLFWwindow* window;
    bool m_show_demo = true;
};

#endif