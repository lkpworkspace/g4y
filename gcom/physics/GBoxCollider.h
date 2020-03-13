#ifndef __GBOXCOLLIDER_H__
#define __GBOXCOLLIDER_H__
#include "GCollider.h"
#include "GPhyWorld.h"

class GTransform;
class GPhyWorld;
class GBoxCollider : public GCollider
{
public:
    GBoxCollider(){}
    virtual ~GBoxCollider(){}

    virtual void Init() override;

    virtual void Start() override;

    virtual void OnDestroy() override;

    virtual std::string ColliderName() { return "GBoxCollider"; }

// private:
    std::weak_ptr<GPhyWorld>                    m_phy_world;
    std::weak_ptr<GTransform>                   m_transform;
};

#endif