#include "GOpenGLView.h"
#include <iostream>
#include "GShader.hpp"
#include "GWorld.h"
#include "GResourceMgr.h"

static void glfw_error_callback(int error, const char* description)
{
    std::cout << "[ERROR] Glfw " << error << " : "<< description << std::endl;
}

int GOpenGLView::Init(const boost::property_tree::ptree& cfg)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 410";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    window = glfwCreateWindow(1280, 854, "G4Y", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = glewInit() != GLEW_OK;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontDefault();

	ShowDemo(false);
	return 0;
}

std::shared_ptr<GShader> GOpenGLView::GetShader()
{ 
	if (m_main_shader == nullptr) {
		m_main_shader = GWorld::Instance()->ResourceMgr()->Shader("main_shader");
	}
	return m_main_shader; 
}

void GOpenGLView::BeginRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        ImGui::StyleColorsClassic();
        static bool show_demo = false;
        if(m_show_demo){
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_MenuBar;
            //window_flags |= ImGuiWindowFlags_NoBackground;

            ImGui::Begin("G4Y Infomation");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Checkbox("show demo", &show_demo);
            if(show_demo)
                ImGui::ShowDemoWindow(&show_demo);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
    }
}

void GOpenGLView::PollEvents()
{
    glfwPollEvents();
}

bool GOpenGLView::WindowShouldClose()
{
    return glfwWindowShouldClose(window);
}

void GOpenGLView::SetCursorPos(double x, double y)
{
    glfwSetCursorPos(window, x, y);
}

void GOpenGLView::GetWindowSize(int& w, int& h)
{
    glfwGetWindowSize(window, &w, &h);
}

void GOpenGLView::SetRenderRect(glm::ivec4 rect)
{
    int w, h;
    GetWindowSize(w, h);
    rect.y = h - rect.z - rect.y;
    glViewport(rect.x, rect.y, rect.w, rect.z);
}

void GOpenGLView::ShowDemo(bool b)
{
    m_show_demo = b;
}

void GOpenGLView::EndRender()
{
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void GOpenGLView::ExitGL()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}