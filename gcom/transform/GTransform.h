#ifndef __GTRANSFORM_H__
#define __GTRANSFORM_H__
#include "GCom.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <iostream>
class GTransform : public GCom
{
public:
    GTransform();
    virtual ~GTransform();

    virtual void Start() override;

    void SetPosition(glm::vec3);
    void SetRotation(glm::vec3 eulers);
    void SetRotation(glm::quat q);
    void RotateAround(glm::vec3 target, glm::vec3 axis, float euler);
    void SetScale(glm::vec3);

    glm::vec3 Position();
    glm::vec3 EulerAngles();
    glm::quat Rotation();
    glm::vec3 Scale();

    glm::vec3 Forward();
    glm::vec3 Right();
    glm::vec3 Up();

    void Translate(glm::vec3 translation);
    void Translate(float x, float y, float z);

    virtual std::string ComName() override { return "GTransform"; }

    glm::mat4 ToMat4();

private:
    void UpdateTransform(std::shared_ptr<GObj> obj, bool update_local);
    void UpdateGlobalTransform(std::shared_ptr<GObj> obj);

    struct Transform{
        glm::quat rot;
        glm::vec3 pos;
        glm::vec3 scale;

        Transform();

        Transform(const glm::vec3& _pos, const glm::quat& _rot, glm::vec3 _scale);

        Transform(const Transform &trans);

        Transform& operator=(const Transform &trans);

        Transform Inverted() const;

        Transform operator*(const Transform& rhs) const;

        glm::mat4 ToMat4() const;
    };

    Transform local_trans;
    Transform wld_trans;

    std::weak_ptr<GTransform> m_parent_trans;
};

#endif

