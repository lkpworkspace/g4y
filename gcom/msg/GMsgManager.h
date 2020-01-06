#ifndef __GMSGMANAGER_H__
#define __GMSGMANAGER_H__
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <queue>
#include <unordered_map>

#include "GCom.h"

template<typename T>
class GMsgManager : public GCom
{
public:
    GMsgManager()
    {}

    virtual ~GMsgManager(){}

    virtual void Init() override
    {
    }

    virtual void Update() override 
    {
        while(!m_msg_queue.empty()){
            auto p = m_msg_queue.front();
            ProcessMsg(p.first, p.second);
            m_msg_queue.pop();
        }
    }
    virtual std::string ComName() override { return "GMsgManager"; }

    void PushMsg(unsigned int cli_id, std::string data)
    {
        m_msg_queue.push(std::make_pair(cli_id, data));
    }

    std::vector<std::pair<unsigned int, std::string>> DispatchMsg()
    {
        std::vector<std::pair<unsigned int, std::string>> d;
        for(const auto& p : m_clis){
            auto vec = p->second->DispatchMsg();

            for(const auto& t : vec){
                GMsgOperator way;
                unsigned int cli_id;
                std::string data;
                std::tie(way, cli_id, data) = t;

                if(way == GMsgOperator::EMO_NORMAL){
                    d.push_back(std::make_pair(cli_id, data));
                }else if(way == GMsgOperator::EMO_BOARDCAST){
                    for(const auto& p : m_clis){
                        d.push_back(std::make_pair(p->first, data));
                    }
                }
            }
        }
    }

    // for client
    std::shared_ptr<T> RegisterClient(unsigned int client_id)
    {
        std::shared_ptr<T> cli = nullptr;
        if(m_clis.find(client_id) == m_clis.end()){
            cli = std::make_shared<T>();
            m_clis[client_id] = cli;
        }
        return cli;
    }

private:
    void ProcessMsg(unsigned int client_id, std::string& data)
    {
        std::shared_ptr<T> cli = nullptr;
        if(m_clis.find(client_id) == m_clis.end()){
            cli = std::make_shared<T>();
            m_clis[client_id] = cli;
        }else{
            cli = m_clis[client_id];
        }
        cli->ProcessMsg(data);
    }

    std::unordered_map<unsigned int, std::shared_ptr<T>> m_clis;
    std::queue<std::pair<unsigned int, std::string>> m_msg_queue;
};


#endif