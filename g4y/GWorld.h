#ifndef __GWORLD_H__
#define __GWORLD_H__
#include <unordered_set>
#include <memory>

class GScene;
class GDynamicsWorld;
class GOpenGLView;
class GWorld : public std::enable_shared_from_this<GWorld>
{
    friend class GScene;
public:
    GWorld();
    virtual ~GWorld();

    void Update();
    void SetScene(std::shared_ptr<GScene> s);

    int Run();
private:
    std::shared_ptr<GScene>         m_scene;
    std::shared_ptr<GDynamicsWorld> m_phy_world;
    std::shared_ptr<GOpenGLView>    m_gl_view;
};

#endif
