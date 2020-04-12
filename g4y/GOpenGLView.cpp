#include "GOpenGLView.h"
#include <iostream>
#include "GShader.hpp"

#if 0
static const char* VS_CODE = \
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

static const char* FS_CODE = \
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture_diffuse1, TexCoords);\n"
"}";

#define GLSL_LOAD false
#else
#define VS_CODE "/home/lkp/projs/gfy/g4y/g4y.vs"
#define FS_CODE "/home/lkp/projs/gfy/g4y/g4y.fs"
#define GLSL_LOAD true
#endif

static void glfw_error_callback(int error, const char* description)
{
    std::cout << "[ERROR] Glfw " << error << " : "<< description << std::endl;
}
int GOpenGLView::InitGL()
{
#ifdef USE_GRAPHICS
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    InitShader();
#endif
}

void GOpenGLView::InitShader()
{
    m_global_shader = std::make_shared<GShader>(VS_CODE, FS_CODE, GLSL_LOAD);
}

void GOpenGLView::BeginRender()
{
#ifdef USE_GRAPHICS
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
            window_flags |= ImGuiWindowFlags_NoBackground;

            ImGui::Begin("G4Y Infomation");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Checkbox("show demo", &show_demo);
            if(show_demo)
                ImGui::ShowDemoWindow(&show_demo);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
    }
#endif
}

void GOpenGLView::PollEvents()
{
#ifdef USE_GRAPHICS
    glfwPollEvents();
#endif
}

bool GOpenGLView::WindowShouldClose()
{
#ifdef USE_GRAPHICS
    return glfwWindowShouldClose(window);
#endif
    return true;
}

void GOpenGLView::SetCursorPos(double x, double y)
{
#ifdef USE_GRAPHICS
    glfwSetCursorPos(window, x, y);
#endif
}

void GOpenGLView::GetWindowSize(int& w, int& h)
{
#ifdef USE_GRAPHICS
    glfwGetWindowSize(window, &w, &h);
#endif
}

void GOpenGLView::SetRenderRect(glm::ivec4 rect)
{
#ifdef USE_GRAPHICS
    int w, h;
    GetWindowSize(w, h);
    rect.y = h - rect.z - rect.y;
    glViewport(rect.x, rect.y, rect.w, rect.z);
#endif
}

void GOpenGLView::ShowDemo(bool b)
{
    m_show_demo = b;
}

void GOpenGLView::EndRender()
{
#ifdef USE_GRAPHICS
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
#endif
}

void GOpenGLView::ExitGL()
{
#ifdef USE_GRAPHICS
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
#endif
}