#ifndef __GCOLLIDER_H__
#define __GCOLLIDER_H__
#include "GCom.h"
#include <iostream>
#include <set>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

class GCollider;
class GCollision
{
public:
    std::shared_ptr<GCollider> collider;
};

class GCollider : public GCom
{
	G_COM
public:

    void OnCollision(const btCollisionObject* col_obj);

    void OnCollisionEnd();

    virtual std::string ColliderName() { return "GCollider"; }

	void SetPostion(glm::vec3);

    std::shared_ptr<btCollisionShape>     m_shape;
    std::shared_ptr<btCollisionObject>    m_col_obj;

    std::set<const btCollisionObject*>          m_last_cols;
    std::set<const btCollisionObject*>          m_cur_cols;
};

#endif