#include "GObj.h"
#include "GCom.h"

#include "GTransform.h"

GObj::GObj()
{
    Init();
}

GObj::GObj(std::shared_ptr<GObj> parent)
{
    Init();
}

GObj::~GObj()
{}

void GObj::Init()
{
    m_active = true;
    AddDefaultComs();
}

void GObj::AddChild(std::shared_ptr<GObj> obj)
{
    m_objs.insert(obj);
}
void GObj::DelChild(std::shared_ptr<GObj> obj)
{
    m_objs.erase(obj);
}

void GObj::AddCom(std::shared_ptr<GCom> com)
{
    com->Init();
    m_coms.insert(com);
}
void GObj::DelCom(std::shared_ptr<GCom> com)
{
    com->Exit();
    m_coms.erase(com);
}

std::shared_ptr<GCom> GObj::GetCom(std::string com_name)
{
    return nullptr;
}

void GObj::UpdateComAndChildren()
{
    if(!m_active) return;

    for(const auto& c : m_coms){
        c->Update();
    }
    for(const auto& o : m_objs){
        if(!o->m_active) continue;
        o->UpdateComAndChildren();
    }
}

void GObj::AddDefaultComs()
{
    AddCom(std::static_pointer_cast<GCom>(std::make_shared<GTransform>()));
}