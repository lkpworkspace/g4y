#include "GBoostMsgQueue.h"
#include <iostream>

bool GBoostMsgQueue::Start(char mod, std::string name)
{
    using namespace boost::interprocess; 
    try{  
        if(mod == 'c'){
            //Erase previous message queue   
            message_queue::remove(name.c_str());  
        }

        //Create a message_queue.   
        m_mq = std::make_shared<message_queue>  
            (((mod == 'c') ? create_only : open_only) 
            ,name.c_str()           
            ,100                       //max message number   
            ,64 * 1024                 //max message size   
            ); 
    }  
    catch(interprocess_exception &ex){  
        std::cout << ex.what() << std::endl;  
        return false;  
    }
    return true;
}