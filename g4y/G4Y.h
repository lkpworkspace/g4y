#ifndef __G4Y_H__
#define __G4Y_H__
#include "GCommon.h"

#include "GWorld.h"
#include "GScene.h"

#include "GCom.h"
#include "GObj.h"

#include "GResourceMgr.h"
#include "GOpenGLView.h"
#include "GPhyWorld.h"

#include "GTransform.h"
#include "GCamera.h"

#include "GRigibody.h"
#include "GBoxCollider.h"
#include "GMeshCollider.h"

#include "GGrid.h"
#include "GAxis.h"
#include "GSkybox.h"
#include "GCube.h"

#include "GShader.hpp"
#include "GMesh.hpp"
#include "GTexture.hpp"
#include "GMaterial.hpp"
#include "GMeshRenderer.hpp"

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