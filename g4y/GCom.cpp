#include "GCom.h"
#include "GObj.h"
#include "GWorld.h"

GCom::GCom() :
    m_start(false)
{}

GCom::~GCom()
{}

void GCom::OnStart()
{
    if(!m_start){
        m_start = true;
        Start();
    }
}

double GCom::GetTime()
{
    return GWorld::GetTime();
}