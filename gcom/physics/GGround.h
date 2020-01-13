#ifndef __GGROUND_H__
#define __GGROUND_H__
#include "GCom.h"
#include "GDynamicsWorld.h"

class GTransform;
class GDynamicsWorld;
class GGround : public GCom
{
public:
    GGround(){}
    virtual ~GGround(){}

    virtual void Awake() override;

    virtual void Update() override;

    virtual void Exit() override;

    virtual std::string ComName() override { return "GGround"; }

private:
    std::weak_ptr<GDynamicsWorld>   m_phy_world;
    std::weak_ptr<GTransform>       m_transform;
    std::shared_ptr<btCollisionShape>     m_ground;
    std::shared_ptr<btDefaultMotionState> m_motion_state;
    std::shared_ptr<btRigidBody>          m_rigidbody;
};

#endif