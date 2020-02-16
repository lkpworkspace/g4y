#include "GScene.h"
#include "GObj.h"
#include "GWorld.h"
#include "GPhyWorld.h"
#include "GOpenGLView.h"

GScene::GScene() :
    std::enable_shared_from_this<GScene>()
{
}

GScene::~GScene()
{}

std::weak_ptr<GScene> GScene::m_cur_scene;
std::shared_ptr<GScene> GScene::CurScene()
{
    return m_cur_scene.lock();
}

std::shared_ptr<GOpenGLView> GScene::GLView() 
{ 
    return m_gl_view.lock(); 
}

void GScene::AddChild(std::shared_ptr<GObj> obj)
{
    m_objs.insert(obj);
}

void GScene::DelChild(std::shared_ptr<GObj> obj)
{
    m_objs.erase(obj);
}

void GScene::Update()
{
    // awake
    for( const auto& go : m_objs ) {
        go->Start();
    }

    // update physics
    m_phy_world.lock()->UpdateDynamicsWorld();
    
    // process event
    m_gl_view.lock()->PollEvents();
    
    m_gl_view.lock()->BeginRender();

    // update logic
    for( const auto& go : m_objs ) {
        go->UpdateComAndChildren();
    }

    // late update logic
    for( const auto& go : m_objs ) {
        go->UpdateLate();
    }

    // rendeing
    for(const auto& go : m_objs){
        go->UpdateRender();
    }
    
    // update gui
    for(const auto& go : m_objs){
        go->UpdateUI();
    }

    m_gl_view.lock()->EndRender();
}