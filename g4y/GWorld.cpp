#include "GWorld.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "GScene.h"
#include "GPhyWorld.h"
#include "GOpenGLView.h"
std::shared_ptr<boost::timer> GWorld::s_timer = std::make_shared<boost::timer>();

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
    return s_timer->elapsed();
}

int GWorld::Run()
{
    while(!m_gl_view->WindowShouldClose()){
        m_scene->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}

void GWorld::SetScene(std::shared_ptr<GScene> s)
{
    m_scene = s;
    s->m_gl_view   = m_gl_view;
    s->m_phy_world = m_phy_world;
    s->m_cur_scene = s;
    s->m_world     = shared_from_this();
}
