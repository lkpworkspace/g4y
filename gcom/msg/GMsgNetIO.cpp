#include "GMsgNetIO.h"
#include "GCom.h"
#include "GObj.h"

bool GSock::Start(bool srv, std::string ip, unsigned int port)
{
    if(srv){

    }else{

    }
}
bool GSock::Start()
{}

void Read(char* buf, int len)
{
    auto self = shared_from_this();
    // m_socket.async_read_some(boost::asio::buffer(buf, len),
    //     [this, self](boost::system::error_code ec, std::size_t length)
    //     {
    //         if (!ec)
    //         {
    //         }
    //     });
}

void Write(char* buf, int len)
{
    auto self = shared_from_this();
    // boost::asio::async_write(m_socket, boost::asio::buffer(buf, len),
    //     [this, self](boost::system::error_code ec, std::size_t /*length*/)
    //     {
    //         if (!ec)
    //         {
    //         }
    //     });
}

/*
    在Awake中
        - 找到GCliMsgMgr或者GSrvMsgMgr
        - 客户端
            - 连接服务端
        - 服务端
            - 启动监听
    在Update中
        - 启动网络事件循环
        - 处理网络事件
        - 将消息添加至GCliMsgMgr或GSrvMsgMgr中
        - 从GCliMsgMgr或GSrvMsgMgr中获得消息并分发出去
    消息解析:
        - 读两个字节头, 获得接下来读到的内容, 解析
    消息打包
        - 填写2个字节的数据包长度,再把消息填进去, 发送
*/

void GMsgNetIO::Accept()
{
    // m_acceptor.async_accept(
    //     [this](boost::system::error_code ec, tcp::socket socket)
    //     {
    //         if (!ec)
    //         {
    //             std::make_shared<session>(std::move(socket))->start();
    //         }

    //         Accept();
    //     });
}

void GMsgNetIO::Awake()
{

}

void GMsgNetIO::Update()
{
    m_io_context.poll();
}
