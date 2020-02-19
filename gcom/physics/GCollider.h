#ifndef __GCOLLIDER_H__
#define __GCOLLIDER_H__
#include "GCom.h"
#include "GPhyWorld.h"
#include <iostream>
#include <set>

class GCollider;
class GCollision
{
public:
    std::shared_ptr<GCollider> collider;
};

class GCollider : public GCom
{
public:

    void OnCollision(const btCollisionObject* col_obj);

    void OnCollisionEnd();

    virtual std::string ComName() { return "GCollider"; }

    virtual std::string ColliderName() { return "GCollider"; }

    std::shared_ptr<btCollisionShape>     m_shape;

    std::set<const btCollisionObject*>          m_last_cols;
    std::set<const btCollisionObject*>          m_cur_cols;
};

#endif