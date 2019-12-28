#include "GWorld.h"
#include "GScene.h"

GWorld::GWorld()
{}

GWorld::~GWorld()
{}

void GWorld::RunWithScene(std::shared_ptr<GScene> s)
{
    s->m_cur_scene = s;
    s->Update();
}
