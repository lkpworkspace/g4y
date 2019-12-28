#ifndef __GOBJ_H__
#define __GOBJ_H__
#include <unordered_set>
#include <memory>

class GCom;
class GWorld;
class GObj
{
    friend class GWorld;
public:
    GObj();
    GObj(std::shared_ptr<GObj> parent);
    virtual ~GObj();

    void SetActive(bool b = true){ m_active = b; }
    bool IsActive(){ return m_active; }

    void AddChild(std::shared_ptr<GObj> obj);
    void DelChild(std::shared_ptr<GObj> obj);

    void AddCom(std::shared_ptr<GCom> com);
    void DelCom(std::shared_ptr<GCom> com);

    std::shared_ptr<GCom> GetCom(std::string com_name);
protected:
    void Init();
    void UpdateComAndChildren();
    void AddDefaultComs();
private:
    bool        m_active;
    //std::string m_name;
    //std::string m_tag;
    //std::string m_layer;
    
    std::unordered_set<std::shared_ptr<GCom>> m_coms;
    std::unordered_set<std::shared_ptr<GObj>> m_objs;
};

#endif