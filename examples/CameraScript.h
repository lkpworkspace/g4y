#ifndef __CameraScript_H__
#define __CameraScript_H__
#include "G4Y.h"

/*
	脚本需要添加至含有GCamera组件的对象中
*/
class CameraScript : public GCom
{
    G_COM
public:
    virtual void Start() override
    {
        m_camera = GetCom<GCamera>();
        auto camera = m_camera.lock();
        ortho_rect = camera->OrthoRect();
        nearfar = camera->NearFar();
        perspective = camera->FovAspect();
    }

    virtual void Update() override
    {
        ImGui::Begin("Camera");

        if(!m_camera.expired()){
            auto camera = m_camera.lock(); 
            bool ortho = camera->IsOrthographic();
            // 设置正视图或透视图
            const char* items[] = { "orthographic", "perspective" };
            static int item_current = ortho ? 0 : 1;
            if(ImGui::Combo("projection", &item_current, items, IM_ARRAYSIZE(items))){
                camera->SetCameraType(item_current == 0 ? GCamera::ORTHO : GCamera::PERSPECTIVE);
            }
            float min = 0.0f, max = 1.0f;
            {
                float dis_min = -10000.0f, dis_max = 10000.0f;
                bool drag_state[2];
                drag_state[0] = ImGui::DragScalar("near",     ImGuiDataType_Float,  &nearfar.x, 0.05f,  &dis_min, &dis_max, "%f", 1.0f);
                drag_state[1] = ImGui::DragScalar(" far",     ImGuiDataType_Float,  &nearfar.y, 0.05f,  &dis_min, &dis_max, "%f", 1.0f);
                if(drag_state[0] || drag_state[1]){
                    camera->SetNearFar(nearfar);
                }
            }
            if(ortho){
                float wh_min = 0.0f, wh_max = 10000.0f;
                bool drag_state[4];
                drag_state[0] = ImGui::DragScalar("X",     ImGuiDataType_Float,  &ortho_rect.x, 0.05f,  &min, &max, "%f", 1.0f);
                drag_state[1] = ImGui::DragScalar("Y",     ImGuiDataType_Float,  &ortho_rect.y, 0.05f,  &min, &max, "%f", 1.0f);
                drag_state[2] = ImGui::DragScalar("W",     ImGuiDataType_Float,  &ortho_rect.z, 0.05f,  &wh_min, &wh_max, "%f", 1.0f);
                drag_state[3] = ImGui::DragScalar("H",     ImGuiDataType_Float,  &ortho_rect.w, 0.05f,  &wh_min, &wh_max, "%f", 1.0f);
                if(drag_state[0] || drag_state[1] || drag_state[2] || drag_state[3]){
                    camera->SetOrthoRect(ortho_rect);
                }
            }else{
                float fov_min = 0.0f, fov_max = 180.0f;
                float aspect_min = 0.0f, aspect_max = 1.0f;
                bool drag_state[2];
                drag_state[0] = ImGui::DragScalar("fov",     ImGuiDataType_Float,  &perspective.x, 0.05f,  &fov_min, &fov_max, "%f", 1.0f);
                drag_state[1] = ImGui::DragScalar("aspect",  ImGuiDataType_Float,  &perspective.y, 0.05f,  &aspect_min, &aspect_max, "%f", 1.0f);
                if(drag_state[0] || drag_state[1] || drag_state[2] || drag_state[3]){
                    camera->SetFovAspect(perspective);
                }
            }
        }

        ImGui::End();  
    }

    glm::vec2 perspective; // x fov, y aspect
    glm::vec4 ortho_rect;
    glm::vec2 nearfar;
    std::weak_ptr<GCamera> m_camera;
};

#endif