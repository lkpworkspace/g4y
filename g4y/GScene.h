#ifndef __GSCENE_H__
#define __GSCENE_H__
#include <memory>
#include <queue>
#include <unordered_set>

#ifdef USE_GUI
class GShader;
class GModel;
#endif
class GObj;
class GWorld;
class GDynamicsWorld;
class GScene : public std::enable_shared_from_this<GScene>
{
    friend class GWorld;
public:
    GScene();
    virtual ~GScene();

    void Update();

    void AddChild(std::shared_ptr<GObj> obj);
    void DelChild(std::shared_ptr<GObj> obj);

    std::shared_ptr<GDynamicsWorld> PhyWorld() { return m_phy_world.lock(); }

    static std::shared_ptr<GScene> CurScene();
#ifdef USE_GUI
    std::shared_ptr<GShader> m_shader;
#endif
private:
    void SetCurScene() { m_cur_scene = shared_from_this(); }
    
    void OnRenderBegin();
    void OnRenderEnd();

    std::unordered_set<std::shared_ptr<GObj>> m_objs;
    std::weak_ptr<GWorld>                     m_world;
    std::weak_ptr<GDynamicsWorld>             m_phy_world;
    static std::weak_ptr<GScene>              m_cur_scene;
};

#endif

