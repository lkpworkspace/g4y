#ifndef __GMESHCOLLIDER_H__
#define __GMESHCOLLIDER_H__
#include "GCollider.h"
#include "GPhyWorld.h"
#include <iostream>

class GTransform;
class GPhyWorld;
class GMeshCollider : public GCollider
{
public:
    GMeshCollider(){}
    virtual ~GMeshCollider(){}

    virtual void Init() override;

    virtual void Start() override;

    virtual void Exit() override;

    virtual std::string ColliderName() { return "GMeshCollider"; }

private:
    std::weak_ptr<GPhyWorld>              m_phy_world;
    std::weak_ptr<GTransform>             m_transform;
    std::shared_ptr<btCollisionObject>    m_col_obj;
};

#endif