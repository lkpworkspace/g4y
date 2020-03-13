#ifndef __CameraScript_H__
#define __CameraScript_H__
#include "G4Y.h"

class CameraScript : public GCom
{
public:
    CameraScript(){}

    CameraScript(glm::vec3 pos) :
        x(pos.x),
        y(pos.y),
        z(pos.z)
    {}

    virtual void Start() override
    {
        Obj()->Transform()->SetPosition(glm::vec3(x,y,z));
        Obj()->Transform()->SetRotation(glm::vec3(rx,ry,rz));
    }

    virtual void Update() override
    {
        ImGui::Begin("Camera");

        bool slider_pos[3];
        slider_pos[0] = ImGui::SliderFloat("Camera x", &x, -150.0f, 150.0f);
        slider_pos[1] = ImGui::SliderFloat("Camera y", &y, -150.0f, 150.0f);
        slider_pos[2] = ImGui::SliderFloat("Camera z", &z, -150.0f, 150.0f);
        if(slider_pos[0] || slider_pos[1] || slider_pos[2])
            Obj()->Transform()->SetPosition(glm::vec3(x,y,z));

        bool slider_rotate[3];
        slider_rotate[0] = ImGui::SliderFloat("Camera rx", &rx, -180.0f, 180.0f);
        slider_rotate[1] = ImGui::SliderFloat("Camera ry", &ry, -180.0f, 180.0f);
        slider_rotate[2] = ImGui::SliderFloat("Camera rz", &rz, -180.0f, 180.0f);
        if(slider_rotate[0] || slider_rotate[1] || slider_rotate[2])
            Obj()->Transform()->SetRotation(glm::vec3(rx,ry,rz));

        ImGui::End();  
    }

    float x = 0.0f;
    float y = 10.0f;
    float z = 15.0f;
    float rx = -20.0f;
    float ry = 0.0f;
    float rz = 0.0f;
};

#endif