#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__
#include "G4Y.h"

class Block : public GCom
{
public:
    virtual void Start() override
    {
        Obj()->SetTag("Block");
    }

};

#endif