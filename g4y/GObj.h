#ifndef __GOBJ_H__
#define __GOBJ_H__
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#include "GScene.h"
#include "GTransform.h"
#include "GCom.h"

class GObj : public std::enable_shared_from_this<GObj>
{
    friend class GScene;
public:
    GObj();
    virtual ~GObj();

    void SetActive(bool b = true){ m_active = b; }
    bool IsActive(){ return m_active; }

    bool AddChild(std::shared_ptr<GObj> obj);
    void DelChild(std::shared_ptr<GObj> obj);

    bool AddCom(std::shared_ptr<GCom> com);

    void DelCom(std::shared_ptr<GCom> com);

    std::shared_ptr<GCom> GetCom(std::string com_name);
    
    void SetTag(std::string tag);
    std::string Tag() { return m_tag; }

    std::shared_ptr<GTransform> Transform(){
        return std::static_pointer_cast<GTransform>(GetCom("GTransform"));
    }

    std::shared_ptr<GScene> Scene(){ return GScene::CurScene(); }

    static std::shared_ptr<GObj> FindWithTag(std::string tag);
    static std::vector<std::shared_ptr<GObj>> FindObjsWithTag(std::string tag);

    void AddDefaultComs();
    
protected:

    void Init();

    void UpdateComAndChildren();

    void UpdateRender();

    void UpdateUI();

private:

    bool         m_active;

    /* for search */
    std::string  m_tag;

    std::weak_ptr<GObj> m_parent;
    
    std::unordered_set<std::shared_ptr<GCom>> m_coms;

    std::unordered_map<std::string, std::weak_ptr<GCom>> m_named_coms;

    std::unordered_set<std::shared_ptr<GObj>> m_children;

    static std::unordered_multimap<std::string, std::weak_ptr<GObj>> s_tagged_objs;
};

#endif