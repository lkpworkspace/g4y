#include "GTransform.h"
#include "GObj.h"

GTransform::Transform::Transform()
: pos(glm::vec3(0,0,0))
, rot2(0, 0, 0)
, rot(glm::vec3(0, 0, 0))
, scale(glm::vec3(1,1,1))
{}

GTransform::Transform::Transform(const glm::vec3& _pos, const glm::quat& _rot, glm::vec3 _scale)
: pos(_pos)
, rot(_rot)
, scale(_scale)
{}

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
    return {rot * (rhs.pos * scale) + pos, rot * rhs.rot, scale * rhs.scale};
}

glm::mat4 GTransform::Transform::ToMat4() const
{
    // auto mtx = glm::mat4_cast(rot);
    // mtx = glm::scale(mtx, scale);
    // mtx[3] = glm::vec4(pos.x, pos.y, pos.z, mtx[3].w);
    // return mtx;
    glm::mat4 t(1.0f);
    glm::mat4 r(1.0f);
    glm::mat4 s(1.0f);

    s = glm::scale(s, scale);

    r = glm::mat4_cast(rot);

    t = glm::translate(t, pos);

    // SRT
    return t * r * s;
}

GTransform::GTransform()
{}

GTransform::~GTransform()
{}

void GTransform::SetPosition(glm::vec3 pos)
{
    wld_trans.pos = pos;
}

void GTransform::SetScale(glm::vec3 scale)
{
    wld_trans.scale = scale;
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
}

glm::vec3 GTransform::Position()
{
    return wld_trans.pos;
}

glm::vec3 GTransform::Scale()
{
    return wld_trans.scale;
}

glm::vec3 GTransform::EulerAngles()
{
    // 角度转弧度 π/180×角度
    // 弧度变角度 180/π×弧度
    auto radians = glm::eulerAngles(wld_trans.rot);
    auto c = (180.0f / glm::pi<float>());
    return glm::vec3(c * radians.x, c * radians.y, c * radians.z);
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