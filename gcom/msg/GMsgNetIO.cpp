#include "GMsgNetIO.h"
#include "GCommon.h"

#include "GMsgMgr.h"
#include "GCliMsgMgr.h"
#include "GSrvMsgMgr.h"
#include "GMsg.h"

boost::asio::io_service g_io_context;
void GSock::Start()
{
    BOOST_LOG_FUNCTION();
    src::severity_logger< severity_level > slg;
    BOOST_LOG_SEV(g_lg::get(), debug) << "sock " << m_id << " start";
    DoReadHead();
}

void GSock::DoReadHead()
{
    boost::asio::async_read(m_sock,
        boost::asio::buffer(&m_len, HEADER_LENGTH),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            BOOST_LOG_FUNCTION();
            if (!ec && m_len < MAX_BODY_LENGTH)
            {
                DoReadBody();
            }
            else
            {
                Close();
                BOOST_LOG_SEV(g_lg::get(), debug) << "sock " << m_id << " " << ec.message();
            }
        });
}

void GSock::DoReadBody()
{
    boost::asio::async_read(m_sock,
        boost::asio::buffer(m_data, m_len),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            BOOST_LOG_FUNCTION();
            if (!ec)
            {
                BOOST_LOG_SEV(g_lg::get(), debug) << "sock recv" << ++m_recv_cnt;
                m_msg_queue.push(std::string(m_data, m_len));
                DoReadHead();
            }
            else
            {
                Close();
                BOOST_LOG_SEV(g_lg::get(), debug) << "sock " << m_id << " " << ec.message();
            }
        });
}

void GSock::DoWrite()
{
    BOOST_LOG_FUNCTION();
    if(m_snd_queue.size() > 20){
        BOOST_LOG_SEV(g_lg::get(), warning) << "sock " << m_id << " write queue size > 20";
        return;
    }
    boost::asio::async_write(m_sock,
        boost::asio::buffer(m_snd_queue.front().data(),m_snd_queue.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            m_snd_queue.pop();
            if (!ec){
                if(!m_snd_queue.empty()){
                    DoWrite();
                }
            }else{
                Close();
                BOOST_LOG_SEV(g_lg::get(), debug) << "sock " << m_id << " " << ec.message();
            }
        });
}

void GSock::Close()
{
    if(m_msg_netio.expired()) return;
    auto& socks = m_msg_netio.lock()->m_socks;
    if(socks.find(m_id) != socks.end()){
        socks.erase(m_id);
    }
}

void GSock::Write(const char* buf, int len)
{
    std::string s(buf, len);
    g_io_context.post(
        [this, s]()
        {
            bool write_in_progress = !m_snd_queue.empty();
            m_snd_queue.push(s);
            if (!write_in_progress)
            {
                DoWrite();
            }
        });
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

    程序设计:
        
*/

GMsgNetIO::GMsgNetIO() :
    std::enable_shared_from_this<GMsgNetIO>(),
    m_init(false),
    m_assign_id(0),
    m_socket(g_io_context)
{
}

unsigned int GMsgNetIO::AssignId()
{
    return m_assign_id++;
}

bool GMsgNetIO::Start(bool srv, std::string ip, unsigned int port)
{
    if(m_init) return false;
    
    m_srv = srv;
    using namespace boost::asio::ip;
    if(srv){
        m_acceptor = std::make_shared<tcp::acceptor>(g_io_context, tcp::endpoint(tcp::v4(), port));
        Accept();
        m_init = true;
    }else{
        std::cout << "[info] begin connect" << std::endl;
        tcp::resolver resolver(g_io_context);
        auto endpoint_iterator = resolver.resolve({ ip, std::to_string(port) });
        unsigned int id = AssignId();
        m_socks.emplace(id, std::make_shared<GSock>(id, std::move(m_socket), shared_from_this()));
        boost::asio::async_connect(m_socket, endpoint_iterator,
        [this](boost::system::error_code ec, tcp::resolver::iterator)
        {
            if (!ec)
            {
                std::cout << "connect ok" << std::endl;
                auto cli = Cli();
                cli->m_sock = std::move(m_socket);
                cli->Start();
                m_init = true;
            }else{
                std::cout << "connect failed" << std::endl;
            }
        });
    }
    return true;
}

void GMsgNetIO::Accept()
{
    using namespace boost::asio::ip;
    m_acceptor->async_accept(m_socket,
        [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                unsigned int id = AssignId();
                m_socks.emplace(id, std::make_shared<GSock>(id, std::move(m_socket), shared_from_this()));
                m_socks[id]->Start();
                std::cout << "get client " << id << std::endl;
            }else{
                std::cout << ec.message() << std::endl;
            }
            Accept();
        });
}

void GMsgNetIO::Init()
{
    Obj()->SetTag("GMsgNetIO");
}

void GMsgNetIO::Awake()
{
    if(m_srv){
        auto msgmgr_obj = Obj()->FindWithTag("GSrvMsgMgr");
        if(msgmgr_obj != nullptr){
            m_srvmsgmgr = msgmgr_obj->GetCom<GSrvMsgMgr>("GSrvMsgMgr");
        }else{
            std::cout << "can not find server msgmgr com " << std::endl;
        }
    }else{
        auto msgmgr_obj = Obj()->FindWithTag("GCliMsgMgr");
        if(msgmgr_obj != nullptr){
            m_climsgmgr = msgmgr_obj->GetCom<GCliMsgMgr>("GCliMsgMgr");
        }else{
            std::cout << "can not find client msgmgr com " << std::endl;
        }
    }
}

void GMsgNetIO::Update()
{
    g_io_context.poll();
    if(!m_init) return;
    // push msg to msgmgr
    for(const auto& p : m_socks){
        auto id = p.first;
        auto cli = p.second;
        while(!cli->m_msg_queue.empty()){
            if(m_srv){
                m_srvmsgmgr.lock()->PushMsg(id, cli->m_msg_queue.front());
                cli->m_msg_queue.pop();
            }else{
                m_climsgmgr.lock()->PushMsg(cli->m_msg_queue.front());
                cli->m_msg_queue.pop();
            }
        }
    }

    // pop msg from msgmgr
    if(m_srv){
        std::string s;
        while(1){
            auto p = m_srvmsgmgr.lock()->PopMsg();
            if(p.first == -1) break;
            if(m_socks.find(p.first) == m_socks.end()) break;
            auto cli = m_socks[p.first];
            unsigned short h = p.second.size();
            s.append((char*)&h, 2);
            s.append(p.second.data(), p.second.size());
            cli->Write(s.data(), s.size());
            s.clear();
        }
    }else{
        std::string s;
        auto cli = Cli();
        if(cli != nullptr){
            while(1){
                auto d = m_climsgmgr.lock()->PopMsg();
                if(d.empty()) break;
                unsigned short h = d.size();
                s.append((char*)&h, 2);
                s.append(d.data(), d.size());
            }
        }
        cli->Write(s.data(), s.size());
    }
}
