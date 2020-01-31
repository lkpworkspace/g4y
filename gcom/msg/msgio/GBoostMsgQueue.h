#ifndef __GBOOSTMSGQUEUE_H__
#define __GBOOSTMSGQUEUE_H__
#include "GMsgIO.h"
#include <memory>
#include <boost/interprocess/ipc/message_queue.hpp>

class GBoostMsgQueue : public GMsgIO
{
public:
    /*
        mod: 'c' 创建消息队列, 'o' 打开消息队列
        name: 消息队列名

        return: true 打开或创建成功, false 打开或创建失败
    */
    bool Start(char mod, std::string name);

    virtual int Read(std::string& dat, int len) override
    {
        using namespace boost::interprocess;
        message_queue::size_type recvd_size;
        unsigned int priority;
        m_mq->receive(dat.data(), len, recvd_size, priority);
    }

    virtual int Write(std::string& dat) override
    {
        m_mq->send(dat.data(), dat.size(), 0);
    }

    std::shared_ptr<boost::interprocess::message_queue> m_mq;
};


#endif