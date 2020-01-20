#ifndef __GSPHERE_H__
#define __GSPHERE_H__
#include "GCom.h"
#include "GPhyWorld.h"

class GTransform;
class GPhyWorld;
class GSphereCollider : public GCom, public btMotionState
{
public:
    GSphereCollider(){}
    virtual ~GSphereCollider(){}

    virtual void Awake() override;

    virtual void Exit() override;

    virtual void getWorldTransform(btTransform& worldTrans ) const override;

    virtual void setWorldTransform(const btTransform& worldTrans) override;

    virtual std::string ComName() override { return "GSphereCollider"; }

// private:
    std::weak_ptr<GPhyWorld>   m_phy_world;
    std::weak_ptr<GTransform>       m_transform;
    std::shared_ptr<btCollisionShape>     m_sphere;
    std::shared_ptr<btRigidBody>          m_rigidbody;
};

#endif