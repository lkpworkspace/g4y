#ifndef __GPBCLIMSGMGR_H__
#define __GPBCLIMSGMGR_H__
#include <memory>
#include "GObj.h"
#include "GCliMsgMgr.h"
#include "GPbMsgFmt.h"

class GPbCliMsgMgr : public GCliMsgMgr
{
public:

    virtual std::string ComName() { return "GPbCliMsgMgr"; }
    
    virtual void Init() override 
    { 
        Obj()->SetTag("GPbCliMsgMgr");
        m_msg_fmt = std::make_shared<GPbMsgFmt>();
    }

    virtual void Update() override;

    virtual void LateUpdate() override
    {
        /* 发送消息 */
        while(!m_send_msgs.empty()){
            // call myframe Send function   
        }
    }

    std::shared_ptr<GPbMsgFmt> m_msg_fmt;
};

#endif