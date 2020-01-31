#ifndef __GMSGMGR_H__
#define __GMSGMGR_H__
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include "GCom.h"
#include "g4y.pb.h"

class GCom;
class GMsg;
class GMsgMgr : public GCom
{
public:
    template<typename T>
    std::shared_ptr<T> CreateCom(std::string com_name, bool loc);
    // msg_name: Protobuf的Message.GetTypeName()
    bool RegMsgCom(std::string msg_name, std::function<std::shared_ptr<GMsg>(bool)>);
    void UnRegMsgCom(std::string);

    // 消息格式：
    //  - 消息头长度(2字节) + 消息头GMetaMsg(不定长) + N个GXXXMsg(不定长)
    std::string Serilize(std::vector<std::shared_ptr<::google::protobuf::Message>>);
    std::vector<std::shared_ptr<::google::protobuf::Message>> Deserilize(const std::string&);

    std::shared_ptr<google::protobuf::Message> CreateProtobufMsg(const std::string&);

    /* 仅用于测试该类的成员方法 */
    static void Test();

    /* 注册的消息组件 */
    std::unordered_map<std::string, std::function<std::shared_ptr<GMsg>(bool)>> m_reg_coms;
};

template<typename T>
std::shared_ptr<T> GMsgMgr::CreateCom(std::string com_name, bool loc)
{
    if(m_reg_coms.find(com_name) != m_reg_coms.end()){
        auto func = m_reg_coms[com_name];
        return std::static_pointer_cast<T>(func(loc));
    }
    return nullptr;
}

#endif