#include "Panel.h"
#include "PanelMgr.h"

PanelNode::PanelNode() :
    std::enable_shared_from_this<PanelNode>(),
    isleft(true),
    type(PANEL_H)
{}

void PanelNode::SetWindowOpt()
{
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNav;
    //window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

int PanelNode::s_panel_cnt = 0;
void PanelNode::Start()
{
    panel_num = s_panel_cnt++;
    SetWindowOpt();
}

std::shared_ptr<PanelNode> PanelNode::CreateInitEmptyPanel()
{
    auto e = std::make_shared<PanelNode>();
    e->panel_mgr = panel_mgr.lock();
    return e;
}

void PanelNode::AddPanelNode(std::shared_ptr<PanelNode> n)
{
	auto obj = std::make_shared<GObj>();
	obj->AddDefaultComs();
	obj->AddCom(n);
	g4y::curscene()->AddChild(obj);
}

void PanelNode::Update()
{
    if(!left.expired() && !right.expired()) return;
    int head_height = 50;
    std::string title = "empty panel ";
    title += std::to_string(panel_num);

    ImGui::SetNextWindowPos(ImVec2(rect.x, rect.y));
    if(panel_idx >= 0){
        ImGui::SetNextWindowSize(ImVec2(rect.w, head_height));
    }else{
        ImGui::SetNextWindowSize(ImVec2(rect.w, rect.z));
    }
    
    ImGui::Begin(title.c_str(), nullptr, window_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Panel"))
        {
            if(!panel_mgr.expired()){
                auto m = panel_mgr.lock();
                auto& ps = m->act_panels;
                for(int i = 0; i < ps.size(); ++i){
                    if(!ps[i].first){
                        if (ImGui::MenuItem(ps[i].second.lock()->PanelName().c_str())) {
                            if(panel_idx >= 0 && panel_idx < ps.size()){
                                ps[panel_idx].first = false;
                            }
                            panel_idx = i;
                            ps[i].first = true;
                        }
                    }
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    

    if(ImGui::Button("Splite H")){
        if(!panel_mgr.expired()){
            auto el = CreateInitEmptyPanel();
            el->panel_idx = panel_idx;
            auto er = CreateInitEmptyPanel();
            if(panel_mgr.lock()->AddPanel(shared_from_this(), PanelNode::PANEL_H, el, er)){
				AddPanelNode(el);
				AddPanelNode(er);
                /*Obj()->AddCom(el);
                Obj()->AddCom(er);*/
            }
        }
    }ImGui::SameLine();
    if(ImGui::Button("Splite V")){
        if(!panel_mgr.expired()){
            auto el = CreateInitEmptyPanel();
            el->panel_idx = panel_idx;
            auto er = CreateInitEmptyPanel();
            if(panel_mgr.lock()->AddPanel(shared_from_this(), PanelNode::PANEL_V, el, er)){
				AddPanelNode(el);
				AddPanelNode(er);
				/*Obj()->AddCom(el);
				Obj()->AddCom(er);*/
            }
        }
    }ImGui::SameLine();
    if(ImGui::Button("Merge")){
        if(!panel_mgr.expired()){
            std::cout << "UnImp" << std::endl;
        }
    }
    ImGui::End();

    if(!panel_mgr.expired()){
        auto& ps = panel_mgr.lock()->act_panels;
        if(panel_idx >= 0 && panel_idx < ps.size() && !ps[panel_idx].second.expired()){
            glm::ivec4 r = glm::ivec4(rect.x, rect.y + head_height, rect.z - head_height, rect.w);
            ps[panel_idx].second.lock()->Resize(r);
        }
    }
}