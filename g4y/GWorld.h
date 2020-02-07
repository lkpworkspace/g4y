#ifndef __GWORLD_H__
#define __GWORLD_H__
#include <unordered_set>
#include <memory>
#include <thread>
#include <chrono>

class GScene;
class GPhyWorld;
class GOpenGLView;
class GWorld : public std::enable_shared_from_this<GWorld>
{
    friend class GScene;
public:
    GWorld();
    virtual ~GWorld();

    void LoadCfg(std::string cfg);

    void SetScene(std::shared_ptr<GScene> s);

    static double GetTime();

    int Run();
    void Poll();
private:
    std::shared_ptr<GScene>                m_scene;
    std::shared_ptr<GPhyWorld>             m_phy_world;
    std::shared_ptr<GOpenGLView>           m_gl_view;
};

#endif
