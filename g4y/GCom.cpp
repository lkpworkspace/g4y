#include "GCom.h"
#include "GObj.h"
#include "GWorld.h"

GCom::GCom() :
    std::enable_shared_from_this<GCom>(),
    m_awake(false)
{}

GCom::~GCom()
{}

void GCom::OnAwake()
{
    if(!m_awake){
        m_awake = true;
        Awake();
    }
}

double GCom::GetTime()
{
    return GWorld::GetTime();
}