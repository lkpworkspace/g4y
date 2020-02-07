#include "GWorld.h"
#include <iostream>
#include "GScene.h"
#include "GPhyWorld.h"
#include "GOpenGLView.h"

auto g_begin_time = std::chrono::system_clock::now();
GWorld::GWorld() :
    std::enable_shared_from_this<GWorld>()
{
    m_gl_view = std::make_shared<GOpenGLView>();
    m_gl_view->InitGL();

    m_phy_world = std::make_shared<GPhyWorld>();
    m_phy_world->InitPhysics();
}

GWorld::~GWorld()
{
    m_gl_view->ExitGL();
}

double GWorld::GetTime()
{
    using namespace std::chrono;
    auto tp = std::chrono::system_clock::now();
    auto d = std::chrono::duration_cast<microseconds>(tp - g_begin_time);
    return (d.count() * 1.0f / 1e6);
}

int GWorld::Run()
{
    while(!m_gl_view->WindowShouldClose()){
        m_scene->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}

void GWorld::Poll()
{
    m_scene->Update();
}

void GWorld::SetScene(std::shared_ptr<GScene> s)
{
    m_scene = s;
    s->m_gl_view   = m_gl_view;
    s->m_phy_world = m_phy_world;
    s->m_cur_scene = s;
    s->m_world     = shared_from_this();
}
