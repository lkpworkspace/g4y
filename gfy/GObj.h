#ifndef __GOBJ_H__
#define __GOBJ_H__
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>

class GCom;
class GScene;
class GObj : public std::enable_shared_from_this<GObj>
{
    friend class GScene;
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

    static std::shared_ptr<GObj> FindObj(std::string tag);

    void AddDefaultComs();
protected:
    void Init();
    void UpdateComAndChildren();
    void UpdateRender();
    void UpdateUI();
private:
    bool         m_active;
    unsigned int m_id;
    /* ... */
    std::string  m_obj_name;
    /* for search */
    std::string  m_tag;
    /* for update action */
    std::string  m_layer;
    
    std::unordered_map<std::string, std::shared_ptr<GCom>> m_named_coms;
    std::unordered_set<std::shared_ptr<GCom>> m_coms;
    std::unordered_set<std::shared_ptr<GObj>> m_children;
};

#endif