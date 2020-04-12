#ifndef __G4Y_H__
#define __G4Y_H__
#include "GCommon.h"

#include "GResourceMgr.h"

#include "GOpenGLView.h"
#include "GTransform.h"
#include "GCamera.h"
#include "GPhyWorld.h"
#include "GRigibody.h"
#include "GBoxCollider.h"
#include "GMeshCollider.h"
#include "GSphereCollider.h"

#ifdef USE_GRAPHICS
#include "GOpenGLView.h"
// #include "GModel.h"
#include "GGrid.h"
#include "GAxis.h"
#include "GSkybox.h"
#include "GCube.h"
#endif

namespace g4y
{
    double gettime();

    double getdeltatime();

    std::shared_ptr<GScene> curscene();

    std::shared_ptr<GPhyWorld> phyworld();

    std::shared_ptr<GOpenGLView> glview();

    std::shared_ptr<GResourceMgr> resourcemgr();
}

#endif