#include "GCube.h"
#include "GShader.h"
#include "GObj.h"
#include "GCamera.h"

static const char* vs_code = \
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"uniform vec4 color;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"out vec4 vcolor;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(pos, 1.0);\n"
"    vcolor = color;\n"
"}";

static const char* fs_code = \
"#version 330 core\n"
"in vec4 vcolor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vcolor;\n"
"}";

float vertices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f, 
    0.5f,  0.5f, -0.5f, 
    0.5f,  0.5f, -0.5f, 
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f, 
    0.5f,  0.5f,  0.5f, 
    0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 

    0.5f,  0.5f,  0.5f,  
    0.5f,  0.5f, -0.5f,  
    0.5f, -0.5f, -0.5f,  
    0.5f, -0.5f, -0.5f,  
    0.5f, -0.5f,  0.5f,  
    0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f, 
    0.5f, -0.5f, -0.5f,  
    0.5f, -0.5f,  0.5f,  
    0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,  
    0.5f,  0.5f,  0.5f,  
    0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

GCube::GCube()
{}

GCube::~GCube()
{}


void GCube::Start()
{    
    m_transform = Obj()->Transform();
    m_camera = std::static_pointer_cast<GCamera>(Obj()->FindWithTag("GCamera")->GetCom("GCamera"));
    m_shader = std::make_shared<GShader>(vs_code, fs_code, false);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0);


}

void GCube::OnRender()
{
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 P = m_camera.lock()->Projection();
    glm::mat4 V = m_camera.lock()->View();
    glm::mat4 M = m_transform.lock()->ToMat4();

    glEnable(GL_BLEND);
    m_shader->Use();
    m_shader->SetUniform("color", color);
    m_shader->SetUniform("projection", P);
    m_shader->SetUniform("view", V);
    m_shader->SetUniform("model", M);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisable(GL_BLEND);
}

void GCube::Exit()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}