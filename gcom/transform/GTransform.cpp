#include "GTransform.h"
#include "GObj.h"

GTransform::Transform::Transform()
: pos(glm::vec3(0,0,0))
, rot(glm::vec3(0, 0, 0))
, scale(glm::vec3(1,1,1))
{}

GTransform::Transform::Transform(const glm::vec3& _pos, const glm::quat& _rot, glm::vec3 _scale)
: pos(_pos)
, rot(_rot)
, scale(_scale)
{}

GTransform::Transform::Transform(const GTransform::Transform &trans)
{
   operator=(trans);
}

GTransform::Transform& GTransform::Transform::operator=(const GTransform::Transform &trans)
{
    pos = trans.pos;
    rot = trans.rot;
    scale = trans.scale;

    return *this;
}

GTransform::Transform GTransform::Transform::Inverted() const
{
    Transform trans;

    trans.rot = glm::conjugate(rot);
    trans.pos = trans.rot * (-pos / scale);
    trans.scale = glm::vec3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
    return trans;
}

GTransform::Transform GTransform::Transform::operator*(const Transform& rhs) const
{
    return {(rot * (rhs.pos * scale)) + pos, rot * rhs.rot, scale * rhs.scale};
}

glm::mat4 GTransform::Transform::ToMat4() const
{
    auto mtx = glm::mat4_cast(rot);
    mtx = glm::scale(mtx, scale);
    //mtx = glm::translate(mtx, pos);
    mtx[3] = glm::vec4(pos.x, pos.y, pos.z, mtx[3].w);
    return mtx;
}

GTransform::GTransform()
{}

GTransform::~GTransform()
{}

void GTransform::SetPosition(glm::vec3 pos)
{
    wld_trans.pos = pos;
    UpdateTransform(Obj(), true);
}

void GTransform::SetScale(glm::vec3 s)
{
     bool parent_valid = (Obj()->Parent() == nullptr) ? false : true;
    if(!parent_valid){
        wld_trans.scale = s;
        UpdateTransform(Obj(), true);
        return;
    }
    local_trans.scale = s;
    UpdateGlobalTransform(Obj());
}

void GTransform::SetRotation(glm::vec3 eulers)
{
    auto rot = glm::quat(glm::vec3(0,0,0));
    rot = glm::rotate(rot, glm::radians(eulers.y), glm::vec3(0, 1, 0));
    rot = glm::rotate(rot, glm::radians(eulers.x), glm::vec3(1, 0, 0));
    rot = glm::rotate(rot, glm::radians(eulers.z), glm::vec3(0, 0, 1));
    SetRotation(rot);
}

void GTransform::SetRotation(glm::quat q)
{
    wld_trans.rot = q;
    UpdateTransform(Obj(), true);
}

void GTransform::RotateAround(glm::vec3 target, glm::vec3 axis, float euler)
{}

void GTransform::Translate(float x, float y, float z)
{
    Translate(glm::vec3(x, y, z));
}

void GTransform::Translate(glm::vec3 translation)
{
    wld_trans.pos += translation;
    UpdateTransform(Obj(), true);
}

glm::vec3 GTransform::Position()
{
    return wld_trans.pos;
}

glm::vec3 GTransform::EulerAngles()
{
    // 角度转弧度 π/180×角度
    // 弧度变角度 180/π×弧度
    auto radians = glm::eulerAngles(wld_trans.rot);
    return glm::vec3(glm::degrees<float>(radians.x), glm::degrees<float>(radians.y), glm::degrees<float>(radians.z));
}

glm::quat GTransform::Rotation()
{
    return wld_trans.rot;
}

glm::vec3 GTransform::Up()
{
    auto q = wld_trans.rot;
    return glm::normalize(mat3_cast(q)[1]);
}

glm::vec3 GTransform::Right()
{
    auto q = wld_trans.rot;
    return glm::normalize(mat3_cast(q)[0]);
}

glm::vec3 GTransform::Forward()
{
    auto q = wld_trans.rot;
    return glm::normalize(mat3_cast(q)[2]);
}

glm::vec3 GTransform::LocalPosition()
{
    return local_trans.pos;
}
glm::vec3 GTransform::LocalEulerAngles()
{
    auto radians = glm::eulerAngles(local_trans.rot);
    return glm::vec3(glm::degrees<float>(radians.x), glm::degrees<float>(radians.y), glm::degrees<float>(radians.z));
}
glm::quat GTransform::LocalRotation()
{
    return local_trans.rot;
}
glm::vec3 GTransform::Scale()
{
    bool parent_valid = (Obj()->Parent() == nullptr) ? false : true;
    if(!parent_valid){
        return wld_trans.scale;
    }
    return local_trans.scale;
}

void GTransform::SetLocalPostion(glm::vec3 pos)
{
    bool parent_valid = (Obj()->Parent() == nullptr) ? false : true;
    if(!parent_valid){
        SetPosition(pos);
        return;
    }
    local_trans.pos = pos;
    UpdateGlobalTransform(Obj());
}
void GTransform::SetLocalRotation(glm::vec3 eulers)
{
    auto rot = glm::quat(glm::vec3(0,0,0));
    rot = glm::rotate(rot, glm::radians(eulers.y), glm::vec3(0, 1, 0));
    rot = glm::rotate(rot, glm::radians(eulers.x), glm::vec3(1, 0, 0));
    rot = glm::rotate(rot, glm::radians(eulers.z), glm::vec3(0, 0, 1));
    SetLocalRotation(rot);
}
void GTransform::SetLocalRotation(glm::quat q)
{
    bool parent_valid = (Obj()->Parent() == nullptr) ? false : true;
    if(!parent_valid){
        SetRotation(q);
        return;
    }
    local_trans.rot = q;
    UpdateGlobalTransform(Obj());
}

glm::mat4 GTransform::ToMat4()
{
    return wld_trans.ToMat4();
}

void GTransform::Start()
{
    auto parent = Obj()->Parent();
    if(parent){
        m_parent_trans = parent->Transform();
    }
    UpdateTransform(Obj(), true);
}

// update DFS
void GTransform::UpdateTransform(std::shared_ptr<GObj> obj, bool update_local)
{
    if(obj == nullptr) return;
    bool parent_valid = (obj->Parent() == nullptr) ? false : true;

    if(update_local && parent_valid){
        local_trans = (obj->Parent()->Transform()->wld_trans.Inverted() * wld_trans);
    }

    auto children = obj->Children();
    for(const auto& o : children){
        o->Transform()->wld_trans = (wld_trans * o->Transform()->local_trans);
        // auto p = o->Transform()->wld_trans.pos;
        // auto e = o->Transform()->EulerAngles();
        // auto s = o->Transform()->Scale();
        // std::cout << "pos   : " << p.x << " " << p.y << " " << p.z << std::endl;
        // std::cout << "euler : " << e.x << " " << e.y << " " << e.z << std::endl;
        // std::cout << "scale : " << s.x << " " << s.y << " " << s.z << std::endl;
        UpdateTransform(o, false);
    }
}

void GTransform::UpdateGlobalTransform(std::shared_ptr<GObj> obj)
{
    wld_trans = obj->Parent()->Transform()->wld_trans * local_trans;
    UpdateTransform(Obj(), true);
}

// void GTransform::Update()
// {
//     if(!m_parent_trans.expired()){
//         wld_postion = m_parent_trans.lock()->wld_postion + postion;
//         wld_rotate  = m_parent_trans.lock()->wld_rotate + rotate;
//         wld_scale   = m_parent_trans.lock()->wld_scale * scale;
//     }else{
//         wld_postion = postion;
//         wld_rotate  = rotate;
//         wld_scale   = scale;
//     }
//     glm::mat4 t(1.0f);
//     glm::mat4 r(1.0f);
//     glm::mat4 s(1.0f);

//     s = glm::scale(s, scale);

//     r = glm::rotate(r, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
//     r = glm::rotate(r, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
//     r = glm::rotate(r, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

//     t = glm::translate(t, postion);

//     // SRT
//     if(m_parent_trans.expired()){
//         model = t * r * s;
//     }else{
//         model = m_parent_trans.lock()->model * t * r * s;
//     }
// }