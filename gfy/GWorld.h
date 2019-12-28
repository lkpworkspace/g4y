#ifndef __GWORLD_H__
#define __GWORLD_H__
#include <unordered_set>
#include <memory>
class GObj;
class GWorld
{
public:
    GWorld();
    virtual ~GWorld();

    void Update();

    void AddObj(std::shared_ptr<GObj> obj);
    void DelObj(std::shared_ptr<GObj> obj);
    
private:
    /* for find/itertor */
    std::unordered_set<std::shared_ptr<GObj>> m_objs;
};

#endif
