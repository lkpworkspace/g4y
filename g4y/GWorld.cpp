#include "GWorld.h"
#include <iostream>
#include "GScene.h"
#include "GPhyWorld.h"
#include "GOpenGLView.h"

std::unique_ptr<GWorld> GWorld::s_instance = nullptr;

int g_sleep_ms = 10;
auto g_begin_time = std::chrono::system_clock::now();
double g_frame_begin = 0.0f;
double g_delta_time = 0.0f;

void GWorld::StaticInit()
{
    s_instance.reset(new GWorld());
}

GWorld::GWorld()
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

double GWorld::GetDeltaTime()
{
    return g_delta_time;
}

std::shared_ptr<GPhyWorld> GWorld::PhyWorld()
{
    return m_phy_world;
}
std::shared_ptr<GOpenGLView> GWorld::GLView()
{
    return m_gl_view;
}

int GWorld::Run()
{
    while(!m_gl_view->WindowShouldClose()){
        g_frame_begin = GetTime();
        m_scene->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(g_sleep_ms));
        g_delta_time = GetTime() - g_frame_begin;
        g_sleep_ms = g_delta_time > 0.017f ? (g_sleep_ms - 1 > 0 ? g_sleep_ms - 1: 1) : g_sleep_ms + 1;
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
    s->m_cur_scene = s;
}
