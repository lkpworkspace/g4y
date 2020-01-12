#ifndef __GCLIMSGMGR_H__
#define __GCLIMSGMGR_H__
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "GObj.h"
#include "GCom.h"

/*
    - 客户端创建网络对象
        - 将该对象加入到cuuid map中
        - 发送创建对象消息给服务端
        - 对象处于未激活状态

    - 客户端更新网络对象
        - 发送更新消息给服务器

    - 客户端删除网络对象
        - 发送删除消息给服务器
        - 从cuuid和suuid中删除该对象

    - 客户端收到服务端消息
        - 通过cuuid查询obj
            - 查找到则是本地对象
                - 如果是创建消息，设置对象的suuid，激活对象
                - 如果是更新消息，将消息发送给该对象
                - 如果是删除，则报错
        - 通过suuid查询对象
            - 查找到则是其他客户端对象
                - 如果是创建，则报错
                - 如果是更新，则将消息发送给该对象
                - 如果是删除，删除对象
            - 未查找到
                - 如果是创建，则创建对象，加入到suuid中
                    - 此时不再发送创建消息
                - 如果是更新，则报错
                - 如果是删除，则报错
*/
class GCliMsgMgr : public GCom
{
public:    

    virtual std::string ComName() override { return "GCliMsgMgr"; }

    /* 被GMsg调用 */
    void AddSendMsg(std::string data)
    {
        m_send_msgs.push(data);
    }

    /* 被myframe调用 */
    void PushMsg(std::string data)
    {
        m_recv_msgs.push(data);
    }
    std::string PopMsg()
    {
        if(m_send_msgs.empty()) return std::string();

        std::string m = m_send_msgs.front();
        m_send_msgs.pop();
        return m;
    }

    void AddLocalMsgObj(std::string uuid, std::shared_ptr<GObj> obj)
    {
        if(m_local_msg_objs.find(uuid) == m_local_msg_objs.end()){
            m_local_msg_objs[uuid] = obj;
        }else{
            std::cout << "[error] add local msg obj" << std::endl;
        }
    }
    void AddRemoteMsgObj(std::string uuid, std::shared_ptr<GObj> obj)
    {
        if(m_remote_msg_objs.find(uuid) == m_remote_msg_objs.end()){
            m_remote_msg_objs[uuid] = obj;
        }else{
            std::cout << "[error] add remote msg obj" << std::endl;
        }
    }
    void DelLocalMsgObj(std::string uuid)
    {
        if(m_local_msg_objs.find(uuid) != m_local_msg_objs.end()){
            m_local_msg_objs.erase(uuid);
        }else{
            std::cout << "[error] del local msg obj" << std::endl;
        }
    }
    void DelRemoteMsgObj(std::string uuid)
    {
        if(m_remote_msg_objs.find(uuid) != m_remote_msg_objs.end()){
            m_remote_msg_objs.erase(uuid);
        }else{
            std::cout << "[error] del remote msg obj" << std::endl;
        }
    }

    /* 客户端创建的消息对象id, 消息对象 */
    std::unordered_map<std::string, std::weak_ptr<GObj>> m_local_msg_objs;
    /* 服务端创建的消息对象id, 消息对象 */
    std::unordered_map<std::string, std::weak_ptr<GObj>> m_remote_msg_objs;
    
    /* 发送消息缓存 */
    std::queue<std::string> m_send_msgs;
    /* 接收消息缓存 */
    std::queue<std::string> m_recv_msgs;
};

#endif