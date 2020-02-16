#ifndef __GSPHERE_H__
#define __GSPHERE_H__
#include "GCollider.h"
#include "GPhyWorld.h"

class GTransform;
class GPhyWorld;
class GSphereCollider : public GCollider
{
public:
    GSphereCollider(){}
    virtual ~GSphereCollider(){}

    virtual void Init() override;

    virtual void Start() override;

    virtual void Exit() override;

    virtual std::string ColliderName() { return "GSphereCollider"; }

// private:
    std::weak_ptr<GPhyWorld>                    m_phy_world;
    std::weak_ptr<GTransform>                   m_transform;
    std::shared_ptr<btCollisionObject>          m_col_obj;
};

#endif