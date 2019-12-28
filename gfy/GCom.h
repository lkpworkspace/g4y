#ifndef __GCOM_H__
#define __GCOM_H__

class GCom
{
public:
    GCom();
    virtual ~GCom();

    virtual void Init(){}

    virtual void Update() = 0;

    virtual void Exit(){}

};

#endif