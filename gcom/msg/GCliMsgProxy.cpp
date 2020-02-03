#include "GCliMsgProxy.h"
#include "GObj.h"


std::shared_ptr<GObj> GCliMsgProxy::MsgObj(std::string id)
{
    if(m_msg_objs.find(id) != m_msg_objs.end()){
        return m_msg_objs[id].lock();
    }
    return nullptr;
}

void GCliMsgProxy::AddObj(std::string id, std::shared_ptr<GObj> obj)
{
    if(m_msg_objs.find(id) == m_msg_objs.end()){
        m_msg_objs[id] = obj;
    }
}

void GCliMsgProxy::DelObj(std::string id)
{
    if(m_msg_objs.find(id) != m_msg_objs.end()){
        m_msg_objs.erase(id);
    }
}

bool GCliMsgProxy::HaveObj(std::string id)
{
    return m_msg_objs.find(id) != m_msg_objs.end();
}