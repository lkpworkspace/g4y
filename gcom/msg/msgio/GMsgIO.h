#ifndef __GMSGIO_H__
#define __GMSGIO_H__
#include <string>

class GMsgIO
{
public:

    /*
        return: -1 failed, >0 OK
    */
    virtual int Read(char*, int len) = 0;
    virtual int Write(char*, int len) = 0;
};

#endif
