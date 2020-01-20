#ifndef __GMESHCOLLIDER_H__
#define __GMESHCOLLIDER_H__
#include "GCom.h"
#include "GPhyWorld.h"

class GTransform;
class GPhyWorld;
class GMeshCollider : public GCom, public btMotionState
{
public:
    GMeshCollider(){}
    virtual ~GMeshCollider(){}

    virtual void Awake() override;

    virtual void Exit() override;

    virtual void getWorldTransform(btTransform& worldTrans ) const override;

    virtual void setWorldTransform(const btTransform& worldTrans) override;

    virtual std::string ComName() override { return "GMeshCollider"; }

private:
    std::weak_ptr<GPhyWorld>              m_phy_world;
    std::weak_ptr<GTransform>             m_transform;
    std::shared_ptr<btCollisionShape>     m_ground;
    std::shared_ptr<btRigidBody>          m_rigidbody;
};

#endif