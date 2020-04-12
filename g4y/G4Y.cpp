#include "G4Y.h"

namespace g4y
{
    double gettime()
    {
        return GWorld::GetTime();
    }

    double getdeltatime()
    {
        return GWorld::GetDeltaTime();
    }

    std::shared_ptr<GScene> curscene()
    {
        return GScene::CurScene();
    }

    std::shared_ptr<GPhyWorld> phyworld()
    {
        return GWorld::Instance()->PhyWorld();
    }

    std::shared_ptr<GOpenGLView> glview()
    {
        return GWorld::Instance()->GLView();
    }

    std::shared_ptr<GResourceMgr> resourcemgr()
    {
        return GWorld::Instance()->ResourceMgr();
    }
}