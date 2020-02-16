#ifndef __GRIGIBODY_H__
#define __GRIGIBODY_H__
#include "GCollider.h"
#include "GPhyWorld.h"

class GTransform;
class GRigibody : public GCom, public btMotionState
{
public:
    virtual void Init() override;

    virtual void Start() override;

    void OnCollision();

    virtual void getWorldTransform(btTransform& worldTrans ) const override;

    virtual void setWorldTransform(const btTransform& worldTrans) override;

    virtual std::string ComName() { return "GRigibody"; }

    std::weak_ptr<GPhyWorld>              m_phy_world;
    std::weak_ptr<GTransform>             m_transform;
    std::shared_ptr<btRigidBody>          m_rigidbody;
};

#endif