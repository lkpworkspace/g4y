#include "G4Y.h"

namespace g4y
{

    bool getkey(int ch){
        auto& io = ImGui::GetIO();
        for (int i = 0; i < io.InputQueueCharacters.Size; i++) { 
            ImWchar c = io.InputQueueCharacters[i];
            if(ch == c) return true;
        }
        return false;
    }

    bool getkeypress(int ch){
        auto& io = ImGui::GetIO();
        for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) 
            if (ImGui::IsKeyPressed(i) && ch == i){ 
                return true;
            }
        return false;
    }

    bool getkeydown(int ch){
        auto& io = ImGui::GetIO();
        for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) 
            if (io.KeysDownDuration[i] >= 0.0f){ 
                if(ch == i) return true;
            }
        return false;
    }

    bool getmousedown(int b){
        auto& io = ImGui::GetIO();
        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) 
            if (io.MouseDownDuration[i] >= 0.0f && i == b){
                return true;
            }
        return false;
    }

    double gettime()
    {
        return GWorld::GetTime();
    }

    double getdeltatime()
    {
        return GWorld::GetDeltaTime();
    }

    std::shared_ptr<GScene> curscene()
    {
        return GScene::CurScene();
    }

    std::shared_ptr<GPhyWorld> phyworld()
    {
        return GWorld::s_instance->PhyWorld();
    }

    std::shared_ptr<GOpenGLView> glview()
    {
        return GWorld::s_instance->GLView();
    }

}