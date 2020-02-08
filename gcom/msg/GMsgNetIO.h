#ifndef __GMSGNetIO_H__
#define __GMSGNetIO_H__
#include <string>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <queue>
#include <unordered_map>
#include <boost/asio.hpp>
#include <mutex>
#include "GCom.h"

// for boost 1.56.0

class GCom;
class GMsgNetIO;
class GSock
{
    enum { HEADER_LENGTH = 2 };
    enum { MAX_BODY_LENGTH = 65535 };
public:
    GSock(unsigned int id, boost::asio::ip::tcp::socket socket, std::shared_ptr<GMsgNetIO> io) :
        m_id(id),
        m_sock(std::move(socket)),
        m_recv_cnt(0)
    {
        m_msg_netio = io;
    }

    void Start();

    void DoReadHead();

    void DoReadBody();

    void DoWrite();

    void Write(const char* buf, int len);

    void Close();

    void Lock() { m_recv_msg_mutex.lock(); }

    void Unlock() { m_recv_msg_mutex.unlock(); }

    unsigned int                       m_recv_cnt;
    std::mutex                         m_recv_msg_mutex;
    std::weak_ptr<GMsgNetIO>           m_msg_netio;
    std::queue<std::string>            m_msg_queue; // recv msg queue
    std::queue<std::string>            m_snd_queue; // send msg queue
    boost::asio::ip::tcp::socket       m_sock;
    unsigned int                       m_id;

    char                               m_data[MAX_BODY_LENGTH];
    unsigned short                     m_len;
};
class GCliMsgMgr;
class GSrvMsgMgr;
class GMsgNetIO : public GCom, public std::enable_shared_from_this<GMsgNetIO>
{
public:
    GMsgNetIO();

    bool Start(bool srv, std::string ip, unsigned int port);

    void Accept();

    virtual void Init() override;

    virtual void Awake() override;

    virtual void Update() override;

    virtual std::string ComName() { return "GMsgNetIO"; }


    unsigned int AssignId();

    std::shared_ptr<GSock> Cli() { return m_socks.end() == m_socks.begin() ? nullptr : m_socks.begin()->second; }

    // for tcp server
    std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
    boost::asio::ip::tcp::socket                    m_socket;
    // tcp socks
    std::unordered_map<unsigned int, std::shared_ptr<GSock>>         m_socks;

    bool                                            m_init;
    unsigned int                                    m_assign_id;
    bool                                            m_srv;
    std::weak_ptr<GCliMsgMgr>                       m_climsgmgr;
    std::weak_ptr<GSrvMsgMgr>                       m_srvmsgmgr;
};

#endif