#ifndef __GCOM_H__
#define __GCOM_H__
#include <string>
#include <memory>
#include <typeinfo>

#include "GObj.h"

#define G_COM \
public: \
virtual std::string TypeName() { return typeid(*this).name(); }

class GObj;
class GPhyWorld;
class GCom
{
	G_COM
    friend class GObj;
public:
    GCom();
    virtual ~GCom();

    /* 初始化函数,在该组件加入到GObj被调用 */
    virtual void Init(){}

    /* 在场景循环前被调用一次,随后不再别调用 */
    virtual void Start(){}

    virtual void OnCollisionEnter(){};
    virtual void OnCollisionStay(){};
    virtual void OnCollisionExit(){};

    virtual void OnTriggerEnter(){};
    virtual void OnTriggerStay(){};
    virtual void OnTriggerExit(){};

    /* 每一帧更新都会被调用 */
    virtual void Update(){}

    /* 在Update之后被调用 */
    virtual void LateUpdate() {}

    virtual void OnRender(){}

    /* 移除场景被调用 */
    virtual void OnDestroy(){}

    std::shared_ptr<GObj> Obj() { assert(!m_obj.expired()); return m_obj.lock(); }
	std::shared_ptr<GObj> Parent() { return Obj()->Parent(); }

	template<typename T>
	std::shared_ptr<T> GetCom() {
		return Obj()->GetCom<T>();
	}

	void SetPyComRef(boost::python::object& com) { m_pycom = com; }
	boost::python::object& GetPyComRef() { return m_pycom; }

	virtual std::string Name() { return "GCom"; }
	virtual void SetName(std::string) {}

protected:
    void OnStart();
private:
    bool                     m_start;
    bool                     m_destroy;
    std::weak_ptr<GObj>      m_obj;
	boost::python::object    m_pycom;
};

class GComWarp
{
public:
	template<typename T>
	std::shared_ptr<T> get() {
		return std::static_pointer_cast<T>(m_com);
	}

	std::string objType() {
		return "cppObj";
	}

	virtual std::string getMethodInfo(const std::string&) { return ""; };

	std::shared_ptr<GCom> m_com;
};

#endif