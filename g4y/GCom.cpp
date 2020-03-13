#include "GCom.h"
#include "GObj.h"
#include "GWorld.h"

GCom::GCom() :
    m_start(false),
    m_destroy(false)
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