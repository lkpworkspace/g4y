#ifndef __GCLIENTPROXY_H__
#define __GCLIENTPROXY_H__
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

#include "GMsg.h"

class GClientProxy
{
public:
    GClientProxy(){}
    virtual ~GClientProxy(){}

    /**
     * operator:
     *  create obj
     *  update obj
     *  delete obj
     */
    virtual void ProcessMsg(std::string& data) = 0;
    
    std::vector<std::tuple<GMsgOperator, unsigned int, std::string>> DispatchMsg()
    {
        std::vector<std::tuple<GMsgOperator, unsigned int, std::string>> ret;
        for(const auto& p : m_msg_coms){
            if(p.second.expired()){
                GMsgOperator way;
                unsigned int cli_id;
                std::string data;
                auto t = p.second.lock()->Serialize();
                std::tie(way, cli_id, data) = t;
                if(way == GMsgOperator::EMO_NONE) continue;
                ret.push_back(t);
            }
        }
        return ret;
    }

    void RegisterMsgCom(std::string com_id, std::shared_ptr<GMsg> com)
    {
        if(m_msg_coms.find(com_id) == m_msg_coms.end()){
            m_msg_coms[com_id] = com;
        }
    }

    void UnregisterMsgCom(std::string com_id)
    {
        if(m_msg_coms.find(com_id) != m_msg_coms.end()){
            m_msg_coms.erase(com_id);
        }
    }

private:
    unsigned int m_id;
    std::unordered_map<std::string, std::weak_ptr<GMsg>> m_msg_coms;
};

#endif