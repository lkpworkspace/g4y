#ifndef __GCLIMSG_H__
#define __GCLIMSG_H__
#include <string>
#include "GCom.h"
#include "GObj.h"

enum class GMsgOperator : int {
    EMO_NONE,
    EMO_NORMAL,
    EMO_BOARDCAST,
    EMO_DESTORY,
};

/* T1: msg mgr, T2: msg */
template<typename T1, typename T2>
class GCliMsg : public GCom
{
public:
    GCliMsg() :
        m_client_id(""),
        m_server_id("")
    {}

    virtual std::string ComName() override { return "GCliMsg"; }

    /* 处理更新消息,被msgmanager调用 */
    virtual void UpdateMsg(std::shared_ptr<T2> msg) = 0;

    /* 发送更新消息,被this调用 */
    virtual void SendMsg(std::shared_ptr<T2> msg) = 0;

    void SetSUUID(std::string uuid){ m_server_id = uuid; }
    std::string& SUUID() { return m_server_id; }

    void SetCUUID(std::string uuid) { m_client_id = uuid; }
    std::string& CUUID() { return m_client_id; }

    std::string m_client_id;
    std::string m_server_id;
    std::weak_ptr<T1> m_msg_mgr;
};

#endif