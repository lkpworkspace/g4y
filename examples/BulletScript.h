#ifndef __BULLETSCRIPT_H__
#define __BULLETSCRIPT_H__
#include "G4Y.h"

class BulletScript : public GCom
{
public:
    BulletScript(glm::vec3 pos, glm::quat rot, float speed = 8.0f) :
        m_speed(speed),
        m_pos(pos),
        m_rot(rot)
    {}

    virtual void Start() override
    {
        m_trans = Obj()->Transform();
        Obj()->Transform()->SetPosition(m_pos);
        Obj()->Transform()->SetRotation(m_rot);
    }

    float Vec3Distance(glm::vec3 p1, glm::vec3 p2)
    {
        auto p = p1 - p2;
        return sqrt(p.x * p.x + p.y *p.y + p.z * p.z);
    }

    virtual void Update() override
    {
        float dis = Vec3Distance(m_trans.lock()->Position(), m_pos);
        if(dis > 10.0f){
            GObj::Destroy(Obj());
            return;
        }

        float delta_time = GWorld::GetDeltaTime();
        m_trans.lock()->Translate(m_trans.lock()->Forward() * delta_time * m_speed * -1.0f);
    }

    float m_speed;
    glm::vec3 m_pos;
    glm::quat m_rot;
    std::weak_ptr<GTransform> m_trans;
};

#endif