#include <iostream>
#include "GObj.h"
#include "GCom.h"
#include "GScene.h"
#include "GTransform.h"
#ifdef USE_GUI
#include "GAxis.h"
#endif

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

std::unordered_multimap<std::string, std::weak_ptr<GObj>> GObj::s_tagged_objs;

GObj::GObj() :
    std::enable_shared_from_this<GObj>(),
    m_tag("Untagged")
{
    Init();
}

GObj::~GObj()
{}

void GObj::Init()
{
    m_active = true;
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); 
    m_uuid = boost::uuids::to_string(a_uuid);
}

void GObj::SetTag(std::string tag)
{
    if(tag == "Untagged") return;
    if(m_tag != "Untagged"){
        std::cout << "[WARNING] SetTag Again" << std::endl;
        return;
    }
    m_tag = tag;
    s_tagged_objs.insert({tag, shared_from_this()});
}

std::shared_ptr<GObj> GObj::FindWithTag(std::string tag)
{
    auto search = s_tagged_objs.find(tag);
    if(search != s_tagged_objs.end()){
        if(!search->second.expired())
            return search->second.lock();
    }
    return nullptr;
}

std::vector<std::shared_ptr<GObj>> GObj::FindObjsWithTag(std::string tag)
{
    std::vector<std::shared_ptr<GObj>> ret;
    auto pr = s_tagged_objs.equal_range (tag); // pair of begin & end iterators returned
    while(pr.first != pr.second)
    {
        if(!pr.first->second.expired()){
            ret.push_back(pr.first->second.lock());
            ++pr.first; // Increment begin iterator
        }else{
            pr.first = s_tagged_objs.erase(pr.first);
        }
    }
    return ret;
}

bool GObj::AddChild(std::shared_ptr<GObj> obj)
{
    m_children.insert(obj);
    obj->m_parent = shared_from_this();
}

void GObj::DelChild(std::shared_ptr<GObj> obj)
{
    m_children.erase(obj);
}

bool GObj::AddCom(std::shared_ptr<GCom> com)
{    
    com->m_obj = shared_from_this();
    com->Init();

    m_coms.insert(com);
    m_named_coms[com->ComName()] = com;
}

void GObj::DelCom(std::shared_ptr<GCom> com)
{
    com->Exit();
    m_named_coms.erase(com->ComName());
    m_coms.erase(com);
}

std::shared_ptr<GCom> GObj::GetCom(std::string com_name)
{
    if(m_named_coms.find(com_name) == m_named_coms.end())
        return nullptr;
    else
        return m_named_coms[com_name].lock();
}

std::vector<std::shared_ptr<GCom>> GObj::GetComs()
{
    std::vector<std::shared_ptr<GCom>> coms;
    for(const auto& c : m_coms){
        coms.push_back(c);
    }
    return coms;
}

std::vector<std::shared_ptr<GObj>> GObj::Children() 
{ 
    return std::vector<std::shared_ptr<GObj>>{m_children.begin(), m_children.end()}; 
}

void GObj::Start()
{
    for(const auto& c : m_coms){
        c->OnStart();
    }
    for(const auto& o : m_children){
        o->Start();
    }
}

void GObj::UpdateComAndChildren()
{
    if(!m_active) return;

    for(const auto& c : m_coms){
        if(!c->m_start) continue;
        c->Update();
    }
    for(const auto& o : m_children){
        o->UpdateComAndChildren();
    }
}

void GObj::UpdateLate()
{
    if(!m_active) return;

    for(const auto& c : m_coms){
        if(!c->m_start) continue;
        c->LateUpdate();
    }
    for(const auto& o : m_children){
        o->UpdateLate();
    }
}

void GObj::UpdateRender()
{
    if(!m_active) return;

    for(const auto& c : m_coms){
        if(!c->m_start) continue;
        c->OnRender();
    }
    for(const auto& o : m_children){
        o->UpdateRender();
    }
}

void GObj::UpdateUI()
{
     if(!m_active) return;

    for(const auto& c : m_coms){
        if(!c->m_start) continue;
        c->OnGUI();
    }
    for(const auto& o : m_children){
        o->UpdateUI();
    }
}

void GObj::AddDefaultComs()
{
    AddCom(std::static_pointer_cast<GCom>(std::make_shared<GTransform>()));
#ifdef USE_GUI
    AddCom(std::make_shared<GAxis>());
#endif
}