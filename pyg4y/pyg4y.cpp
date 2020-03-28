#include <boost/python.hpp>
#include "G4Y.h"
#include "RoamScript.h"

using namespace boost::python;

void demo()
{
    const auto w = GWorld::Instance();
    auto s = std::make_shared<GScene>();
    w->SetScene(s);

    auto  camera = std::make_shared<GObj>();
    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());

    auto obj = std::make_shared<GObj>();
    obj->AddDefaultComs();

    s->AddChild(camera);
    s->AddChild(obj);
    
    w->Run();
}

class GObjWarp;
class GSceneWarp;
class GWorldWarp;

class GPyCom;

class GComPtr
{
public:
    template<typename T>
    std::shared_ptr<T> get(){
        return std::static_pointer_cast<T>(m_com);
    }

    std::string objType(){
        return "cppObj";
    }

    std::shared_ptr<GCom> m_com;
};

class RoamScriptWarp : public GComPtr
{
public:
    RoamScriptWarp(){
        m_com = std::make_shared<RoamScript>();
    }

    RoamScriptWarp(std::shared_ptr<RoamScript> c){
        m_com = c;
    }

    RoamScriptWarp(const RoamScriptWarp& o){
        operator=(o);
    }

    RoamScriptWarp& operator=(const RoamScriptWarp& o){
        m_com = o.m_com;
        return *this;
    }
};

class GCubeWarp : public GComPtr
{
public:
    GCubeWarp(){
        m_com = std::make_shared<GCube>();
    }

    GCubeWarp(std::shared_ptr<GCube> c){
        m_com = c;
    }

    GCubeWarp(const GCubeWarp& o){
        operator=(o);
    }

    GCubeWarp& operator=(const GCubeWarp& o){
        m_com = o.m_com;
        return *this;
    }
};

class GGridWarp : public GComPtr
{
public:
    GGridWarp(){
        m_com = std::make_shared<GGrid>(-100, 100, 1);
    }

    GGridWarp(std::shared_ptr<GGrid> c){
        m_com = c;
    }

    GGridWarp(const GGridWarp& o){
        operator=(o);
    }

    GGridWarp& operator=(const GGridWarp& o){
        m_com = o.m_com;
        return *this;
    }
};

class GCameraWarp : public GComPtr
{
public:
    GCameraWarp(){
        m_com = std::make_shared<GCamera>();
    }

    GCameraWarp(std::shared_ptr<GCamera> c){
        m_com = c;
    }

    GCameraWarp(const GCameraWarp& o){
        operator=(o);
    }

    GCameraWarp& operator=(const GCameraWarp& o){
        m_com = o.m_com;
        return *this;
    }
};

class GTransformWarp : public GComPtr
{
public:
    GTransformWarp(){
        m_com = std::make_shared<GTransform>();
    }

    GTransformWarp(std::shared_ptr<GTransform> t){
        m_com = t;
    }

    GTransformWarp(const GTransformWarp& o){
        operator=(o);
    }

    GTransformWarp& operator=(const GTransformWarp& o){
        m_com = o.m_com;
        return *this;
    }

    void setPosition(float x, float y, float z){
        get<GTransform>()->SetPosition(glm::vec3(x, y, z));
    }

    object getPosition(){
        auto pos = get<GTransform>()->Position();
        list l;
        l.append(pos.x);
        l.append(pos.y);
        l.append(pos.z);
        return l;
    }

    void setRotation(float x, float y, float z){
        get<GTransform>()->SetRotation(glm::vec3(x, y, z));
    }
};

class GObjWarp
{
public:
    GObjWarp(){
        m_obj = std::make_shared<GObj>();
        m_obj->AddDefaultComs();
        // std::cout << "obj construct 1" << std::endl;
    }

    GObjWarp(std::shared_ptr<GObj> o){
        m_obj = o;
        // std::cout << "obj construct 2" << std::endl;
    }

    GObjWarp(const GObjWarp& o){
        // std::cout << "obj copy" << std::endl;
        operator=(o);
    }

    ~GObjWarp(){
        // std::cout << "obj de" << std::endl;
    }

    GObjWarp& operator=(const GObjWarp& o){
        // std::cout << "obj =" << std::endl;
        m_obj = o.m_obj;
        return *this;
    }

    void addChild(GObjWarp o){
        m_obj->AddChild(o.m_obj);
    }

    bool addCom(object c){
        std::string type = extract<std::string>(c.attr("objType")());
        if(type == "pyObj"){
            // std::cout << "addCom(object cp)" << std::endl;
            auto com = std::make_shared<GPyCom>(c);
            return m_obj->AddCom(com);
        }else if(type == "cppObj"){
            // std::cout << "addCom(GComPtr cp)" << std::endl;
            GComPtr& com = extract<GComPtr&>(c);
            return m_obj->AddCom(com.m_com);
        }
        return false;
    }

    void setTag(str tag){
        m_obj->SetTag(extract<std::string>(tag));
    }

    // static bool destroy(object c){

    // }

    std::shared_ptr<GObj> m_obj;
};

class GSceneWarp
{
public:
    GSceneWarp(){
        m_scene = std::make_shared<GScene>();
    }

    GSceneWarp(const GSceneWarp& o){
        operator=(o);
    }

    GSceneWarp& operator=(const GSceneWarp& o){
        m_scene = o.m_scene;
        return *this;
    }

    void addChild(GObjWarp o){
        m_scene->AddChild(o.m_obj);
    }

    std::shared_ptr<GScene> m_scene;
};

class GWorldWarp
{
public:
    int run(){
        return GWorld::Instance()->Run();
    }

    void setScene(GSceneWarp s){
        GWorld::Instance()->SetScene(s.m_scene);
    }

    void poll(){
        GWorld::Instance()->Poll();
    }
};

class GPyComBase
{
public:
    GPyComBase()
    {
        // std::cout << "py com base construct" << std::endl;
    }

    ~GPyComBase(){
        // std::cout << "py com base deconstruct" << std::endl;
    }

    std::string objType(){
        return "pyObj";
    }

    GObjWarp obj(){
        return GObjWarp(m_obj.lock());
    }

    GTransformWarp transform(){
        return GTransformWarp(m_trans.lock());
    }

    std::weak_ptr<GObj>       m_obj;
    std::weak_ptr<GTransform> m_trans;
};


class GPyCom : public GCom
{
public:
    GPyCom(object o) :
        pyo(o)
    {}

    virtual void Init() override {
        GPyComBase& com = extract<GPyComBase&>(pyo);
        com.m_obj = Obj();
        com.m_trans = Obj()->Transform();

        if(PyObject_HasAttrString(pyo.ptr(),"init"))
            pyo.attr("init")();
    }

    virtual void Start() override {
        if(PyObject_HasAttrString(pyo.ptr(),"start"))
            pyo.attr("start")();
    }

    virtual void Update() override {
        if(PyObject_HasAttrString(pyo.ptr(),"update"))
            pyo.attr("update")();
    }

    virtual void LateUpdate() override {
        if(PyObject_HasAttrString(pyo.ptr(),"lateUpdate"))
            pyo.attr("lateUpdate")();
    }

    virtual void OnDestroy() override {
        if(PyObject_HasAttrString(pyo.ptr(),"onDestroy"))
            pyo.attr("onDestroy")();
    }

    virtual std::string ComName() override { 
        if(PyObject_HasAttrString(pyo.ptr(),"comName"))
            return extract<std::string>(pyo.attr("comName")());
        return "GPyCom";
    }

    object pyo;
};

double deltaTime()
{
    return GWorld::GetDeltaTime();
}

BOOST_PYTHON_MODULE(g4y)
{
    def("demo", demo);
    def("deltaTime", deltaTime);

    class_<GPyComBase>("GCom")
        .def("objType", &GPyComBase::objType)
        .def("obj", &GPyComBase::obj)
        .def("transform", &GPyComBase::transform)
    ;

    class_<GComPtr>("GComPtr")
        .def("objType", &GComPtr::objType)
    ;

    class_<RoamScriptWarp, bases<GComPtr>>("RoamScript")
    ;

    class_<GCameraWarp, bases<GComPtr>>("GCamera")
    ;

    class_<GCubeWarp, bases<GComPtr>>("GCube")
    ;

    class_<GGridWarp, bases<GComPtr>>("GGrid")
    ;

    class_<GTransformWarp, bases<GComPtr>>("GTransform")
        .def("setPosition", &GTransformWarp::setPosition)
        .def("getPosition", &GTransformWarp::getPosition)
        .def("setRotation", &GTransformWarp::setRotation)
    ;

    class_<GObjWarp>("GObj")
        .def("addChild", &GObjWarp::addChild)
        .def("addCom", &GObjWarp::addCom)
        .def("setTag", &GObjWarp::setTag)
    ;

    class_<GSceneWarp>("GScene")
        .def("addChild", &GSceneWarp::addChild)
    ;

    class_<GWorldWarp>("GWorld")
        .def("setScene",&GWorldWarp::setScene)
        .def("run", &GWorldWarp::run)
        .def("poll", &GWorldWarp::poll)
    ;
}
