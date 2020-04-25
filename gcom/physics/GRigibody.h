#ifndef __GRIGIBODY_H__
#define __GRIGIBODY_H__
#include "GCollider.h"
#include "GPhyWorld.h"
#include <set>

class GTransform;
class GRigibody : public GCom, public btMotionState
{
    G_COM
public:
    virtual void Init() override;

    virtual void Start() override;

    void OnCollision(const btCollisionObject* col_obj);

    void OnCollisionEnd();

    virtual void getWorldTransform(btTransform& worldTrans ) const override;

    virtual void setWorldTransform(const btTransform& worldTrans) override;

    std::weak_ptr<GPhyWorld>              m_phy_world;
    std::weak_ptr<GTransform>             m_transform;
    std::shared_ptr<btRigidBody>          m_rigidbody;

    std::set<const btCollisionObject*>          m_last_cols;
    std::set<const btCollisionObject*>          m_cur_cols;
};

#endif