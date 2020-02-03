#ifndef __GMSGNetIO_H__
#define __GMSGNetIO_H__
#include <string>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "GCom.h"

class GCom;
class GSock
{
public:
    bool Start(bool srv, std::string ip, unsigned int port);
    bool Start();

    void Read(char* buf, int len);

    void Write(char* buf, int len);

    boost::asio::ip::tcp::socket       m_sock;
};
class GMsgNetIO : public GCom
{
public:
    bool Start(bool srv, std::string ip, unsigned int port);

    void Accept();

    virtual void Awake() override;

    virtual void Update() override;

private:
    boost::asio::io_service            m_io_context;
    boost::asio::ip::tcp::socket       m_sock;
    boost::asio::ip::tcp::acceptor     m_acceptor;
};

#endif