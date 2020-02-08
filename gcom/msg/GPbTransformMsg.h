#ifndef __GPBTRANSFORMMSG_H__
#define __GPBTRANSFORMMSG_H__
#include "GMsg.h"

class GMsg;
class GTransform;
class GPbTransformMsg : public GMsg
{
public:
    GPbTransformMsg(bool loc) :
        GMsg(loc),
        m_parse_cnt(0),
        m_tmp(0)
    {}
    
    virtual void Init() override;

    virtual void Update() override;

    virtual std::string MsgName() override { return "GTransformMsg"; }

    virtual std::shared_ptr<::google::protobuf::Message> BuildMsg() override;

    virtual void ParseMsg(std::shared_ptr<::google::protobuf::Message>) override;

    virtual std::string ComName() { return "GTransformMsg"; }

private:
    std::weak_ptr<GTransform> m_transform;
    unsigned int              m_parse_cnt;
    unsigned int              m_tmp;
};

#endif