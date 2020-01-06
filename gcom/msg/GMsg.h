#ifndef __GMSG_H__
#define __GMSG_H__
#include <string>
#include "GCom.h"

enum class GMsgOperator : int {
    EMO_NONE,
    EMO_NORMAL,
    EMO_BOARDCAST,
    EMO_DESTORY,
};

class GMsg : public GCom
{
public:
    GMsg(){}
    virtual ~GMsg(){}

    virtual void Init() override {}

    virtual std::string ComName() override { return "GMsg"; }

    virtual std::string Type() = 0;

    virtual std::tuple<GMsgOperator, unsigned int, std::string> Serialize() = 0;

    virtual void Deserialize(std::string& data) = 0;

private:
    std::string m_guid;
    unsigned int m_cli_id;
};

#endif