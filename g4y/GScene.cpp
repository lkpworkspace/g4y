#include "GScene.h"
#include "GObj.h"
#include "GWorld.h"
#ifdef USE_GUI
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GModel.h"

static const char* vs_code = \
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"    TexCoords = aTexCoords;\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}";

static const char* fs_code = \
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture_diffuse1, TexCoords);\n"
"}";
#endif

GScene::GScene() :
    std::enable_shared_from_this<GScene>()
{
#ifdef USE_GUI
    m_shader = std::make_shared<GShader>(vs_code, fs_code, false);
#endif
}

GScene::~GScene()
{}

std::weak_ptr<GScene> GScene::m_cur_scene;
std::shared_ptr<GScene> GScene::CurScene()
{
    return m_cur_scene.lock();
}

void GScene::AddChild(std::shared_ptr<GObj> obj)
{
    m_objs.insert(obj);
}

void GScene::DelChild(std::shared_ptr<GObj> obj)
{
    m_objs.erase(obj);
}

void GScene::OnRenderBegin()
{
#ifdef USE_GUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        static bool show_demo = false;
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::Begin("G4Y Infomation", NULL, window_flags);                          // Create a window called "Hello, world!" and append into it.
        ImGui::Checkbox("show demo", &show_demo);
        if(show_demo)
            ImGui::ShowDemoWindow(&show_demo);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
#endif
    
}

void GScene::OnRenderEnd()
{
#ifdef USE_GUI
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_world.lock()->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_world.lock()->window);
#endif
}

void GScene::Update()
{
    // update physics
    // ...
    
    // process event
#ifdef USE_GUI
    glfwPollEvents();
#endif

    // update logic
    for( const auto& go : m_objs ) {
        go->UpdateComAndChildren();
    }

    OnRenderBegin();

    // rendeing
    for(const auto& go : m_objs){
        go->UpdateRender();
    }
    
    // update gui
    for(const auto& go : m_objs){
        go->UpdateUI();
    }
    OnRenderEnd();
}