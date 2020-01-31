#include "GMsgMgr.h"
#include <iostream>
#include "GCom.h"
#include "GMsg.h"

bool GMsgMgr::RegMsgCom(std::string msg_name, std::function<std::shared_ptr<GMsg>(bool)> crt_func)
{
    if(m_reg_coms.find(msg_name) == m_reg_coms.end()){
        m_reg_coms[msg_name] = crt_func;
        return true;
    }
    return false;
}

void GMsgMgr::UnRegMsgCom(std::string com_name)
{
    if(m_reg_coms.find(com_name) != m_reg_coms.end()){
        m_reg_coms.erase(com_name);
    }
}

std::shared_ptr<google::protobuf::Message> GMsgMgr::CreateProtobufMsg(const std::string& type_name)
{
    google::protobuf::Message* message = NULL;
    const google::protobuf::Descriptor* descriptor =
            google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
    if (descriptor)
    {
        const google::protobuf::Message* prototype =
                google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype)
        {
            message = prototype->New();
        }
    }
    return std::shared_ptr<google::protobuf::Message>(message);
}

// GMetaMsg在下标0, 其它均为组件消息
std::string GMsgMgr::Serilize(std::vector<std::shared_ptr<::google::protobuf::Message>> msgs)
{   
    using namespace ::google::protobuf;
    bool ret;
    unsigned short head_len;
    std::string data;

    if(msgs.empty()) return std::string();
    
    head_len = msgs[0]->ByteSize();
    data.append((const char*)&head_len, 2);
    for(int i = 0; i < msgs.size(); ++i){
        data.append(msgs[i]->SerializeAsString());
    }

    return data;
}

// GMetaMsg在下标0, 其它均为组件消息
std::vector<std::shared_ptr<::google::protobuf::Message>> GMsgMgr::Deserilize(const std::string& dat)
{
    using namespace ::google::protobuf;
    std::vector<std::shared_ptr<::google::protobuf::Message>> msgs;
    /*
        1. Get head_len
        2. Parse GMetaMsg
        3. Get OtherMsg
    */
    bool ret;
    int idx = 0;
    std::shared_ptr<Message> msg = nullptr;
    const char* d = dat.data();
    unsigned short head_len;
    memcpy(&head_len, &d[idx], 2); idx += 2;

    auto meta = std::make_shared<GMetaMsg>();
    ret = meta->ParseFromArray(&d[idx], head_len); idx += head_len;
    if(!ret){
        return msgs;
    }
    msgs.push_back(meta);

    for(int i = 0; i < meta->info_size(); ++i){
        auto info = meta->info(i);
        auto msg  = CreateProtobufMsg(info.name());
        msg->ParseFromArray(&d[idx], info.sz());
        idx += info.sz();
        msgs.push_back(msg);
    }

    return msgs;
}


////////////////////////////////////////////////////////////////////////////////
// test code
///////////////////////////////////////////////////////////////////////////////

void GMsgMgr::Test()
{
    class TestMsg : public GMsg
    {
    public:
        virtual std::shared_ptr<::google::protobuf::Message> BuildMsg() override
        {}

        virtual void ParseMsg(std::shared_ptr<::google::protobuf::Message>) override
        {}

        virtual std::string MsgName() { return "TestMsg"; }

        virtual std::string ComName() { return "TestMsg"; }
    };
    GMsgMgr msgmgr;
    // 测试流程:
    //  注册组件, 创建组件, 删除组件, 创建组件
    std::cout << "[test] reg msg com TestMsg" << std::endl;
    if(false == msgmgr.RegMsgCom("TestMsg", [](bool loc){
        return std::static_pointer_cast<GMsg>(std::make_shared<TestMsg>());
    })){
        std::cout << "[test] reg msg com failed" << std::endl;
        return;
    }
     
    auto test_msg = msgmgr.CreateCom<TestMsg>("TestMsg", true);
    if(test_msg == nullptr){
        std::cout << "[test] find TestMsg com failed" << std::endl;
        return;
    }
    std::cout << "[test] create msg com " << test_msg->ComName() << std::endl;

    std::cout << "[test] unreg msg com TestMsg" << std::endl;
    msgmgr.UnRegMsgCom("TestMsg");

    std::cout << "[test] create TestMsg" << std::endl;
    test_msg = msgmgr.CreateCom<TestMsg>("TestMsg", true);
    if(test_msg != nullptr){
        return;
    }
    std::cout << "[test] find TestMsg com failed" << std::endl;
    std::cout << "[test] Test reg/create/remove msg com OK" << std::endl;

    // 测试流程
    //  创建消息对象序列化,然后再反序列化,对比结果
    auto meta = std::make_shared<GMetaMsg>();
    meta->set_parent_loc_id("parent_loc_id");
    meta->set_parent_srv_id("parent_srv_id");
    meta->set_loc_id("loc_id");
    meta->set_srv_id("srv_id");

    auto msg = std::make_shared<GTransformMsg>();
    msg->set_posx(1);
    msg->set_posy(2);
    msg->set_posz(3);
    msg->set_rx(4);
    msg->set_ry(5);
    msg->set_rz(6);
    msg->set_sx(7);
    msg->set_sy(8);
    msg->set_sz(9);

    auto info = std::make_shared<GMsgInfo>();
    info->set_name(msg->GetTypeName());
    info->set_sz(msg->ByteSize());
    info->set_action(GMsgInfo_Action::GMsgInfo_Action_CREATE);

    auto info_ptr = meta->add_info();
    info_ptr->set_action(info->action());
    info_ptr->set_name(info->name());
    info_ptr->set_sz(info->sz());

    std::vector<std::shared_ptr<::google::protobuf::Message>> msgs;
    std::vector<std::shared_ptr<::google::protobuf::Message>> msgs2;
    msgs.push_back(meta);
    msgs.push_back(msg);
    std::string d = msgmgr.Serilize(msgs);
    msgs2 = msgmgr.Deserilize(d);

    std::cout << std::endl;
    for(int i = 0; i < msgs2.size(); ++i){
        std::cout << msgs2[i]->DebugString() << std::endl;
    }
    std::cout << "[test] Test Serilize/Deserilize OK" << std::endl;
}