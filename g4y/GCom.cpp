#include "GCom.h"
#include "GObj.h"

GCom::GCom() :
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