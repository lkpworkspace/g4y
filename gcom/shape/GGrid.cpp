#include "GGrid.h"
#include "GShader.hpp"
#include "GObj.h"
#include "GCamera.h"

static const char* vs_code = \
"#version 330 core\n"
"layout (location = 0) in vec2 pos;\n"
"uniform vec4 color;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"out vec4 vcolor;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(pos.x, 0, pos.y, 1.0);\n"
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

GGrid::GGrid(int begin, int end, int step) :
    m_begin(begin),
    m_end(end),
    m_step(step),
    m_arr(nullptr)
{}

GGrid::~GGrid()
{}

void GGrid::CreateGridArr()
{
    m_cnt = (m_end - m_begin + 1) / m_step * 2 * 2 * 2;
    m_arr = std::shared_ptr<float[]>(new float[m_cnt]);

    int idx = 0;
    int idx2 = 0;
    for(int i = m_begin; i <= m_end; i += m_step){
        m_arr[idx] = -100.0f;
        m_arr[idx + 1] = i * 1.0f;
        m_arr[idx + 2] = 100.0f;
        m_arr[idx + 3] = i * 1.0f;

        idx2 = m_cnt / 2 + idx;
        m_arr[idx2] = i * 1.0f;
        m_arr[idx2 + 1] = -100.0f;
        m_arr[idx2 + 2] = i * 1.0f;
        m_arr[idx2 + 3] = 100.0f;
        idx += 4;
    }
}

void GGrid::Init()
{
    CreateGridArr();
    
    m_camera = std::static_pointer_cast<GCamera>(Obj()->FindWithTag("GCamera")->GetCom<GCamera>());
    // 编译加载编译shader
    m_shader = std::make_shared<GShader>(vs_code, fs_code, false);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, m_cnt * sizeof(float), m_arr.get(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


}

void GGrid::Update()
{

}

void GGrid::OnRender()
{
    glm::vec4 color(1.0f, 1.0f, 1.0f, 0.1f);
    glm::mat4 P = m_camera.lock()->Projection();
    glm::mat4 V = m_camera.lock()->View();
    glm::mat4 M = glm::mat4(1.0f);

    m_shader->Use();
    m_shader->SetUniform("color", color);
    m_shader->SetUniform("projection", P);
    m_shader->SetUniform("view", V);
    m_shader->SetUniform("model", M);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_LINES, 0, m_cnt);

    glDisable(GL_BLEND);
}

void GGrid::OnDestroy()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}