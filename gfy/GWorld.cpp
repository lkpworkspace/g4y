#include "GWorld.h"
#include "GObj.h"

GWorld::GWorld()
{}

GWorld::~GWorld()
{}

void GWorld::AddObj(std::shared_ptr<GObj> obj)
{
    m_objs.insert(obj);
}

void GWorld::DelObj(std::shared_ptr<GObj> obj)
{
    m_objs.erase(obj);
}

void GWorld::Update()
{
    // update all gobj
    for( const auto& go : m_objs ) {
        go->UpdateComAndChildren();
    }
}
