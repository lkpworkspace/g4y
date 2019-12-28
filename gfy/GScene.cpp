#include "GScene.h"
#include "GObj.h"

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

std::shared_ptr<GObj> GScene::Find(std::string obj_name)
{
    // TODO
    return nullptr;
}

unsigned int GScene::AssignId()
{
    unsigned int id = -1;
    if(!m_vaild_ids.empty()){
        id = m_vaild_ids.front();
        m_vaild_ids.pop();
    }else{
        id = m_assign_id++;
    }
    return id;
}

void GScene::FreeId(unsigned int id)
{
    m_vaild_ids.push(id);
}

void GScene::Update()
{
    // update physics
    // ...
    
    // process event
    // ...

    // update logic
    for( const auto& go : m_objs ) {
        go->UpdateComAndChildren();
    }

    // rendeing
    for(const auto& go : m_objs){
        go->UpdateRender();
    }
    
    // update gui
    for(const auto& go : m_objs){
        go->UpdateUI();
    }
}