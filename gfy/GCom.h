#ifndef __GCOM_H__
#define __GCOM_H__
#include <string>
#include <memory>

class GObj;
class GCom
{
    friend class GObj;
public:
    GCom();
    virtual ~GCom();

    virtual std::string GetComName() = 0;

    virtual void Init(){}

    virtual void Update() = 0;

    virtual void OnRender(){}

    virtual void OnGUI(){}

    virtual void Exit(){}

    std::shared_ptr<GObj> Obj() { return m_obj.lock(); }

private:
    std::weak_ptr<GObj> m_obj;
};

#endif