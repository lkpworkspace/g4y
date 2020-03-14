#ifndef __G4Y_H__
#define __G4Y_H__
#include "GWorld.h"
#include "GScene.h"

#include "GCom.h"
#include "GObj.h"

#include "GResourceMgr.h"

#include "GTransform.h"
#include "GCamera.h"
#include "GPhyWorld.h"
#include "GRigibody.h"
#include "GMeshCollider.h"
#include "GSphereCollider.h"

#ifdef USE_GUI
#include "GOpenGLView.h"
#include "GModel.h"
#include "GGrid.h"
#include "GAxis.h"
#include "GSkybox.h"
#include "GCube.h"
#endif

namespace g4y
{

    bool getkey(int ch);

    bool getkeypress(int ch);
    
    bool getkeydown(int ch);

    bool getmousedown(int b = 1);

    double gettime();

    double getdeltatime();

    std::shared_ptr<GScene> curscene();

    std::shared_ptr<GPhyWorld> phyworld();

    std::shared_ptr<GOpenGLView> glview();
}

#endif