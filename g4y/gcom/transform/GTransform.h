#ifndef __GTRANSFORM_H__
#define __GTRANSFORM_H__
#include "GCom.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"
#include <iostream>

class G4Y_DLL GTransform : public GCom
{
    G_COM
public:
    GTransform();
    virtual ~GTransform();

    virtual void Start() override;

    glm::vec3 Position();
    glm::vec3 EulerAngles();
    glm::quat Rotation();

    void SetPosition(glm::vec3);
    void SetRotation(glm::vec3 eulers);
    void SetRotation(glm::quat q);
    void RotateAround(glm::vec3 target, glm::vec3 axis, float euler);

    glm::vec3 LocalPosition();
    glm::vec3 LocalEulerAngles();
    glm::quat LocalRotation();

    void SetLocalPostion(glm::vec3);
    void SetLocalRotation(glm::vec3);
    void SetLocalRotation(glm::quat q);

    void LookAt(glm::vec3 target, glm::vec3 wld_up = glm::vec3(0, 1, 0));

    glm::vec3 Forward();
    glm::vec3 Right();
    glm::vec3 Up();

    void Translate(glm::vec3 translation);
    void Translate(float x, float y, float z);

    glm::mat4 ToMat4();

    glm::vec3 Scale();
    void SetScale(glm::vec3);

	virtual std::string Name() { return m_name; }
	virtual void SetName(std::string name) { m_name = name; }
private:
    void UpdateTransform(std::shared_ptr<GObj> obj, bool update_local);
    void UpdateGlobalTransform(std::shared_ptr<GObj> obj);

    struct G4Y_DLL Transform{
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
	std::string m_name;
    std::weak_ptr<GTransform> m_parent_trans;
};

class GTransformWarp : public GComWarp
{
public:
	GTransformWarp() {
		std::cout << "transform warp construct1" << std::endl;
		m_com = std::make_shared<GTransform>();
	}

	GTransformWarp(std::shared_ptr<GTransform> t) {
		std::cout << "transform warp construct2" << std::endl;
		m_com = t;
	}

	GTransformWarp(const GTransformWarp& o) {
		std::cout << "transform warp construct3" << std::endl;
		operator=(o);
	}

	GTransformWarp& operator=(const GTransformWarp& o) {
		std::cout << "transform warp construct4" << std::endl;
		m_com = o.m_com;
		return *this;
	}

	virtual std::string getMethodInfo(const std::string&) override;

	void setPosition(float x, float y, float z) {
		get<GTransform>()->SetPosition(glm::vec3(x, y, z));
	}

	boost::python::object getPosition() {
		auto pos = get<GTransform>()->Position();
		boost::python::list l;
		l.append(pos.x);
		l.append(pos.y);
		l.append(pos.z);
		return l;
	}

	void setEulerAngles(float x, float y, float z) {
		get<GTransform>()->SetRotation(glm::vec3(x, y, z));
	}

	boost::python::object getEulerAngles() {
		auto pos = get<GTransform>()->EulerAngles();
		boost::python::list l;
		l.append(pos.x);
		l.append(pos.y);
		l.append(pos.z);
		return l;
	}

	void setScale(float x, float y, float z) {
		get<GTransform>()->SetScale(glm::vec3(x, y, z));
	}

	boost::python::object getScale() {
		auto pos = get<GTransform>()->Scale();
		boost::python::list l;
		l.append(pos.x);
		l.append(pos.y);
		l.append(pos.z);
		return l;
	}

	boost::python::object getForward() {
		auto pos = get<GTransform>()->Forward();
		boost::python::list l;
		l.append(pos.x);
		l.append(pos.y);
		l.append(pos.z);
		return l;
	}

	boost::python::object getRight() {
		auto pos = get<GTransform>()->Right();
		boost::python::list l;
		l.append(pos.x);
		l.append(pos.y);
		l.append(pos.z);
		return l;
	}

	boost::python::object getUp() {
		auto pos = get<GTransform>()->Up();
		boost::python::list l;
		l.append(pos.x);
		l.append(pos.y);
		l.append(pos.z);
		return l;
	}

	void translate(float x, float y, float z) {
		get<GTransform>()->Translate(x, y, z);
	}
};

#endif

