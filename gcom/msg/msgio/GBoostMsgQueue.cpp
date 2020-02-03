#include "GBoostMsgQueue.h"
#include <iostream>
#include <thread>
#include <chrono>

bool GBoostMsgQueue::Start(char mod, std::string name, int queue_len, int msg_sz)
{
    using namespace boost::interprocess; 
    try{  
        if(mod == 'c'){
            //Erase previous message queue   
            message_queue::remove(name.c_str());  
        }

        //Create a message_queue.   
        if(mod == 'c'){
            m_mq = std::make_shared<message_queue>  
                (create_only
                ,name.c_str()           
                ,queue_len                       //max message number   
                ,msg_sz                      //max message size   
                ); 
        }else{
            m_mq = std::make_shared<message_queue>(open_only, name.c_str()); 
        }
    }  
    catch(interprocess_exception &ex){  
        std::cout << ex.what() << std::endl;  
        return false;  
    }
    return true;
}

void GBoostMsgQueue::Test(bool srv)
{
    int wnum = 100;
    int rnum;
    GBoostMsgQueue bmq;
    bmq.Start('c', "test_msg_queue", 100, 4);
    bmq.Write((char*)&wnum, sizeof(int));

    GBoostMsgQueue bmq2;
    bmq2.Start('o', "test_msg_queue", 100, 4);
    bmq2.Read((char*)&rnum, sizeof(int));
    std::cout << rnum << std::endl;
}