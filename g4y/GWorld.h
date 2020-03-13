#ifndef __GWORLD_H__
#define __GWORLD_H__
#include <unordered_set>
#include <memory>
#include <thread>
#include <chrono>

class GScene;
class GPhyWorld;
class GOpenGLView;
class GWorld
{
    friend class GScene;
public:

    static void StaticInit();

    static std::unique_ptr< GWorld > s_instance;

    void LoadCfg(std::string cfg);

    void SetScene(std::shared_ptr<GScene> s);

    std::shared_ptr<GPhyWorld> PhyWorld();
    std::shared_ptr<GOpenGLView> GLView();

    /* second */
    static double GetTime();

    /* second */
    static double GetDeltaTime();

    int Run();
    void Poll();

    ~GWorld();
private:
    GWorld();

    std::shared_ptr<GScene>                m_scene;
    std::shared_ptr<GPhyWorld>             m_phy_world;
    std::shared_ptr<GOpenGLView>           m_gl_view;
};

#endif
