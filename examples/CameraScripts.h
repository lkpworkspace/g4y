#ifndef __CameraScripts_H__
#define __CameraScripts_H__
#include "G4Y.h"

class CameraScripts : public GCom
{
public:

    virtual void OnGUI() override
    {
        ImGui::Begin("Camera Move");

        ImGui::SliderFloat("Camera x", &x, -150.0f, 150.0f);
        ImGui::SliderFloat("Camera y", &y, -150.0f, 150.0f);
        ImGui::SliderFloat("Camera z", &z, -150.0f, 150.0f);

        ImGui::SliderFloat("Camera rx", &rx, -180.0f, 180.0f);
        ImGui::SliderFloat("Camera ry", &ry, -180.0f, 180.0f);
        ImGui::SliderFloat("Camera rz", &rz, -180.0f, 180.0f);

        ImGui::End();
    }

    virtual void Update() override
    {
        Obj()->Transform()->postion = glm::vec3(x,y,z);
        Obj()->Transform()->rotate = glm::vec3(rx,ry,rz);
    }

    float x = 0.0f;
    float y = 10.0f;
    float z = 15.0f;
    float rx = -20.0f;
    float ry = 0.0f;
    float rz = 0.0f;
};

#endif