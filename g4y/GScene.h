#ifndef __GSCENE_H__
#define __GSCENE_H__
#include <memory>
#include <queue>
#include <unordered_set>

class GObj;
class GWorld;
class GOpenGLView;
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

    std::shared_ptr<GOpenGLView> GLView();

    static std::shared_ptr<GScene> CurScene();

private:
    void SetCurScene() { m_cur_scene = shared_from_this(); }

    std::unordered_set<std::shared_ptr<GObj>> m_objs;
    std::weak_ptr<GWorld>                     m_world;
    std::weak_ptr<GDynamicsWorld>             m_phy_world;
    std::weak_ptr<GOpenGLView>                m_gl_view;
    static std::weak_ptr<GScene>              m_cur_scene;
};

#endif

