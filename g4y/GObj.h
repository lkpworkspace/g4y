#ifndef __GOBJ_H__
#define __GOBJ_H__
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#include "GScene.h"
#include "GTransform.h"
#include "GCom.h"

class GPhyWorld;
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

    template<typename T>
    bool AddCom(std::shared_ptr<T> com){
        return AddCom(std::static_pointer_cast<GCom>(com));
    }
    template<typename T>
    void DelCom(std::shared_ptr<T> com){
        DelCom(std::static_pointer_cast<GCom>(com));
    }
    bool AddCom(std::shared_ptr<GCom> com);
    void DelCom(std::shared_ptr<GCom> com);

    template<typename T>
    std::shared_ptr<T> GetCom(std::string com_name){
        auto com = GetCom(com_name);
        if(com == nullptr) return nullptr;
        return std::dynamic_pointer_cast<T>(com);
    }
    std::shared_ptr<GCom> GetCom(std::string com_name);
    
    void SetTag(std::string tag);
    std::string Tag() { return m_tag; }

    std::shared_ptr<GObj> Parent() { return m_parent.lock(); }

    std::shared_ptr<GTransform> Transform(){
        return std::static_pointer_cast<GTransform>(GetCom("GTransform"));
    }

    std::shared_ptr<GScene> Scene(){ return GScene::CurScene(); }

    std::shared_ptr<GPhyWorld> PhyWorld(){ return GScene::CurScene()->PhyWorld(); }

    void AddDefaultComs();

    std::string& UUID() { return m_uuid; }

    static std::shared_ptr<GObj> FindWithTag(std::string tag);
    static std::vector<std::shared_ptr<GObj>> FindObjsWithTag(std::string tag);

protected:

    void Init();

    void Awake();

    void UpdateComAndChildren();

    void UpdateLate();

    void UpdateRender();

    void UpdateUI();

private:

    bool         m_active;

    std::string m_uuid;

    /* for search */
    std::string  m_tag;

    std::weak_ptr<GObj> m_parent;
    
    std::unordered_set<std::shared_ptr<GCom>> m_coms;

    std::unordered_map<std::string, std::weak_ptr<GCom>> m_named_coms;

    std::unordered_set<std::shared_ptr<GObj>> m_children;

    static std::unordered_multimap<std::string, std::weak_ptr<GObj>> s_tagged_objs;
};

#endif