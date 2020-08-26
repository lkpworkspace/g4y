#ifndef __GRIGIBODY_H__
#define __GRIGIBODY_H__
#include "GCollider.h"

class btRigidBody;
class btCollisionObject;
class GTransform;
class GPhyWorld;

struct G4Y_DLL GRigidBodyDes final
{
	float mass; //Note: mass equals zero means static, default 1
	glm::vec3 local_inertia; //default (0, 0, 0)
	std::weak_ptr<GCollider> shape;
	bool          disable_sleep; //it is always active if disabled

	explicit GRigidBodyDes(std::shared_ptr<GCollider> c)
		: mass(1.f)
		, local_inertia(0.f, 0.f, 0.f)
		, disable_sleep(true)
		, shape(c)
	{}

	explicit GRigidBodyDes(const GRigidBodyDes& o)
	{
		operator=(o);
	}

	GRigidBodyDes& operator=(const GRigidBodyDes &o)
	{
		mass = o.mass;
		local_inertia = o.local_inertia;
		disable_sleep = o.disable_sleep;
		shape = o.shape.lock();
		return *this;
	}
};

class G4Y_DLL GRigibody final : public GCom
{
	friend class GPhyWorld;
    G_COM
public:
	explicit GRigibody(GRigidBodyDes& des);
	explicit GRigibody(std::shared_ptr<GCollider> c);

    virtual void Init() override;

    virtual void Start() override;

	virtual void OnDestroy() override;

    void OnCollision(const btCollisionObject* col_obj);

    void OnCollisionEnd();

    std::weak_ptr<GPhyWorld>              m_phy_world;
    std::weak_ptr<GTransform>             m_transform;
    btRigidBody*                          m_rigidbody;

    std::set<const btCollisionObject*>          m_last_cols;
    std::set<const btCollisionObject*>          m_cur_cols;

private:
   /**
     * synchronize node transformation to physics
     */
    void SyncNodeToPhysics();
    
    /**
     * synchronize physics transformation to node
     */
    void SyncPhysicsToNode();

	GRigidBodyDes m_des;
};

#endif