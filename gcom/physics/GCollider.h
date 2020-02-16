#ifndef __GCOLLIDER_H__
#define __GCOLLIDER_H__
#include "GCom.h"
#include "GPhyWorld.h"

class GCollider : public GCom
{
public:

    void OnCollision();

    virtual std::string ComName() { return "GCollider"; }

    virtual std::string ColliderName() { return "GCollider"; }

    std::shared_ptr<btCollisionShape>     m_shape;
};

#endif