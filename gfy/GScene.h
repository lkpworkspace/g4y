#ifndef __GSCENE_H__
#define __GSCENE_H__
#include <memory>
#include <queue>
#include <unordered_set>

class GObj;
class GWorld;
class GScene : public std::enable_shared_from_this<GScene>
{
    friend class GWorld;
public:
    GScene() :
        std::enable_shared_from_this<GScene>()
    {}
    virtual ~GScene(){}

    void Update();

    void AddChild(std::shared_ptr<GObj> obj);
    void DelChild(std::shared_ptr<GObj> obj);

    std::shared_ptr<GObj> Find(std::string obj_name);
    unsigned int AssignId();
    void FreeId(unsigned int id);

    void SetCurScene() { m_cur_scene = shared_from_this(); }
    static std::shared_ptr<GScene> CurScene();
private:
    std::unordered_set<std::shared_ptr<GObj>> m_objs;
    std::queue<unsigned int>                  m_vaild_ids;
    unsigned int                              m_assign_id;
    static std::weak_ptr<GScene>              m_cur_scene;
};

#endif