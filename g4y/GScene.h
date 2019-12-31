#ifndef __GSCENE_H__
#define __GSCENE_H__
#include <memory>
#include <queue>
#include <unordered_set>

#ifdef USE_GUI
class GShader;
class GModel;
#endif
class GObj;
class GWorld;
class GScene : public std::enable_shared_from_this<GScene>
{
    friend class GWorld;
public:
    GScene();
    virtual ~GScene();

    void Update();

    void AddChild(std::shared_ptr<GObj> obj);
    void DelChild(std::shared_ptr<GObj> obj);

    static std::shared_ptr<GScene> CurScene();
#ifdef USE_GUI
    std::shared_ptr<GShader> m_shader;
#endif
private:
    void SetCurScene() { m_cur_scene = shared_from_this(); }
    
    void OnRenderBegin();
    void OnRenderEnd();

    std::unordered_set<std::shared_ptr<GObj>> m_objs;
    std::weak_ptr<GWorld>                     m_world;
    static std::weak_ptr<GScene>              m_cur_scene;
};

#endif

