#ifndef __GMSGIO_H__
#define __GMSGIO_H__
#include <string>

class GMsgIO
{
public:

    /*
        return: -1 failed, >0 OK
    */
    virtual int Read(std::string& dat, int len) = 0;
    virtual int Write(std::string& dat) = 0;
};

#endif
