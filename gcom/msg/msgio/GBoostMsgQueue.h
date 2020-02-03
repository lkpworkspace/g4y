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
    bool Start(char mod, std::string name, int queue_len, int msg_sz);

    virtual int Read(char* buf, int len) override
    {
        using namespace boost::interprocess;
        message_queue::size_type recvd_size;
        unsigned int priority;
        m_mq->receive(buf, len, recvd_size, priority);
    }

    virtual int Write(char* buf, int len) override
    {
        m_mq->send(buf, len, 0);
    }

    static void Test(bool srv);

    std::shared_ptr<boost::interprocess::message_queue> m_mq;
};


#endif