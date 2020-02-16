#include "GTransform.h"
#include "GObj.h"

GTransform::GTransform() :
    postion(0, 0, 0),
    rotate(0, 0, 0),
    scale(1, 1, 1)
{
    model = glm::mat4(1.0f);
}

GTransform::~GTransform()
{}

void GTransform::SetPostion(float x, float y, float z)
{
    postion.x = x;
    postion.y = y;
    postion.z = z;
}

void GTransform::SetPostion(glm::vec3& vec)
{
    postion = vec;
}

void GTransform::Start()
{
    auto parent = Obj()->Parent();
    if(parent){
        m_parent_trans = parent->Transform();
    }
}

void GTransform::Update()
{
    if(!m_parent_trans.expired()){
        wld_postion = m_parent_trans.lock()->wld_postion + postion;
        wld_rotate  = m_parent_trans.lock()->wld_rotate + rotate;
        wld_scale   = m_parent_trans.lock()->wld_scale * scale;
    }else{
        wld_postion = postion;
        wld_rotate  = rotate;
        wld_scale   = scale;
    }
    glm::mat4 t(1.0f);
    glm::mat4 r(1.0f);
    glm::mat4 s(1.0f);

    s = glm::scale(s, scale);

    r = glm::rotate(r, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    r = glm::rotate(r, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    r = glm::rotate(r, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

    t = glm::translate(t, postion);

    // SRT
    if(m_parent_trans.expired()){
        model = t * r * s;
    }else{
        model = m_parent_trans.lock()->model * t * r * s;
    }
}