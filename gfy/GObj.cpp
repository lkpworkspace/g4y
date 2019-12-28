#include "GObj.h"
#include "GCom.h"
#include "GScene.h"
#include "GTransform.h"
#include <iostream>

GObj::GObj() :
    std::enable_shared_from_this<GObj>()
{
    Init();
}

GObj::GObj(std::shared_ptr<GObj> parent) :
    std::enable_shared_from_this<GObj>()
{
    Init();
}

GObj::~GObj()
{
    GScene::CurScene()->FreeId(m_id);
}

void GObj::Init()
{
    m_active = true;
    m_id = GScene::CurScene()->AssignId();
}

void GObj::AddChild(std::shared_ptr<GObj> obj)
{
    m_children.insert(obj);
}
void GObj::DelChild(std::shared_ptr<GObj> obj)
{
    m_children.erase(obj);
}

void GObj::AddCom(std::shared_ptr<GCom> com)
{    
    com->m_obj = shared_from_this();
    com->Init();

    m_coms.insert(com);
    m_named_coms[com->GetComName()] = com;
}

void GObj::DelCom(std::shared_ptr<GCom> com)
{
    com->Exit();
    m_named_coms.erase(com->GetComName());
    m_coms.erase(com);
}

std::shared_ptr<GCom> GObj::GetCom(std::string com_name)
{
    return m_named_coms.find(com_name) == m_named_coms.end() ? nullptr : m_named_coms[com_name];
}

void GObj::UpdateComAndChildren()
{
    if(!m_active) return;

    for(const auto& c : m_coms){
        c->Update();
    }
    for(const auto& o : m_children){
        if(!o->m_active) continue;
        o->UpdateComAndChildren();
    }
}

void GObj::UpdateRender()
{
    if(!m_active) return;

    for(const auto& c : m_coms){
        c->OnRender();
    }
}

void GObj::UpdateUI()
{
     if(!m_active) return;

    for(const auto& c : m_coms){
        c->OnGUI();
    }
}

void GObj::AddDefaultComs()
{
    AddCom(std::static_pointer_cast<GCom>(std::make_shared<GTransform>()));
}

std::shared_ptr<GObj> GObj::FindObj(std::string tag)
{
    return GScene::CurScene()->Find(tag);
}