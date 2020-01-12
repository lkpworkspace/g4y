#ifndef __GPBMSGFMT_H__
#define __GPBMSGFMT_H__
#include <string>
#include <memory>
#include "GMsgFmt.h"
#include "g4y.pb.h"

// 消息格式：
//  - 消息头长度(2字节) + 消息头(不定长) + 消息内容(不定长)
class GPbMsgFmt : public GMsgFmt<::google::protobuf::Message>
{
public:
    virtual std::string Serilize(std::shared_ptr<::google::protobuf::Message> msg) override
    {   
        using namespace ::google::protobuf;
        bool b;
        unsigned short head_len;
        GMetaMsg meta;
        std::string meta_data;
        std::string data;

        meta.set_msg_name(msg->GetTypeName());
        b = msg->SerializeToString(&data);
        if(!b){
            return std::string();
        }
        meta.set_data_size(data.size());

        b = meta.SerializeToString(&meta_data);
        if(!b){
            return std::string();
        }

        std::string d;
        head_len = meta_data.size();
        d.append((const char*)&head_len, 2);
        d.append(meta_data);
        d.append(data);

        return d;
    }

    template<typename T>
    std::string Serilize(std::shared_ptr<T> msg){
        return Serilize(std::static_pointer_cast<::google::protobuf::Message>(msg));
    }

    virtual std::shared_ptr<::google::protobuf::Message> Deserilize(std::string dat) override
    {
        using namespace ::google::protobuf;
        bool b;
        std::shared_ptr<Message> msg = nullptr;
        const char* d = dat.data();
        unsigned short head_len;
        memcpy(&head_len, d, 2);

        GMetaMsg meta;
        b = meta.ParseFromArray(&d[2], head_len);
        if(!b){
            return msg;
        }

        std::string msg_name = meta.msg_name();
        if(msg_name == "GProxyMsg"){
            msg = std::static_pointer_cast<Message>(std::make_shared<GProxyMsg>());
        }else if(msg_name == "GTransformMsg"){
            msg = std::static_pointer_cast<Message>(std::make_shared<GTransformMsg>());
        }else{
            return nullptr;
        }
        b = msg->ParseFromArray(&d[2 + head_len], meta.data_size());
        if(!b){
            return nullptr;
        }
        return msg;
    }

    template<typename T>
    std::shared_ptr<T> Deserilize(std::string dat)
    {
        return std::static_pointer_cast<T>(Deserilize(dat));
    }
};

#endif