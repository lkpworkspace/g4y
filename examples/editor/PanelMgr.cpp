#include "PanelMgr.h"
#include "Panel.h"

PanelMgr::PanelMgr() :
    std::enable_shared_from_this<PanelMgr>()
{}

void PanelMgr::Start()
{
    root = std::make_shared<PanelNode>();
    root->panel_mgr = shared_from_this();
    Obj()->AddCom(root);

    int w, h;
    g4y::glview()->GetWindowSize(w, h);
    window_size = glm::ivec2(w, h);
    root->rect = glm::ivec4(0,0,h,w);
    root->isleft = true;
}

void PanelMgr::UnregPanel(std::shared_ptr<PanelNode> p)
{

}

bool PanelMgr::PosInRect(glm::ivec2 pos, glm::ivec4 rect)
{
    return (pos.x >= rect.x) && (pos.x <= (rect.x + rect.w)) && (pos.y >= rect.y) && (pos.y <= rect.y + rect.z);
}

std::shared_ptr<PanelNode> PanelMgr::IsPosInSplitLine(std::shared_ptr<PanelNode> panel, glm::ivec2 pos)
{
    if(!panel) return nullptr;
    if(panel->left.expired() && panel->right.expired()) return nullptr;

    glm::ivec4 rect;
    if(panel->type == PanelNode::PANEL_H){
        rect = glm::ivec4(panel->rect.x + panel->div_pos - 4, panel->rect.y, panel->rect.z, 8);
    }else{
        rect = glm::ivec4(panel->rect.x, panel->rect.y + panel->div_pos - 4, 8, panel->rect.w);
    }
    if(PosInRect(pos, rect)) return panel;

    auto pl = IsPosInSplitLine(panel->left.lock(), pos);
    auto pr = IsPosInSplitLine(panel->right.lock(), pos);

    return pl ? pl : pr;
}

// 测试函数
std::shared_ptr<PanelNode> PanelMgr::GetLeftLeaf(std::shared_ptr<PanelNode> panel)
{
    if(!panel) return nullptr;
    if(panel->left.expired() && panel->right.expired()) return panel;

    return GetLeftLeaf(panel->left.lock());
}

void PanelMgr::Update()
{
#if 0
    ImGui::Begin("PanelMgr");
    static bool ishorizontal = true;
    ImGui::Checkbox("Is Horizontal", &ishorizontal);
    if(ImGui::Button("Add empty panel")){
        auto el = std::make_shared<EmptyPanel>();
        auto er = std::make_shared<EmptyPanel>();

        Obj()->AddCom(el);
        Obj()->AddCom(er);

        auto leaf = GetLeftLeaf(root);
        AddPanel(leaf, ishorizontal ? Panel::PANEL_H : Panel::PANEL_V, el, er);
    }

    ImGui::End();
#endif
    // 获得要移动的panel
    if(ImGui::IsMouseClicked(0)){
        auto pos = ImGui::GetMousePos();
        auto panel = IsPosInSplitLine(root, glm::ivec2(pos.x, pos.y));
        if(panel) click_node = panel;
        else click_node.reset();
    }
    // 释放要移动的panel
    if(ImGui::IsMouseReleased(0)){
        click_node.reset();
    }
    // 移动
    if(ImGui::IsMouseDown(0) && !click_node.expired()){
        static int in_cnt = 0;
        auto& io = ImGui::GetIO();
        int delta = (click_node.lock()->type == PanelNode::PANEL_H) ? io.MouseDelta.x : io.MouseDelta.y;
        // std::cout << "in " << in_cnt++ << " move delta " << delta << std::endl; 
        if(delta != 0){
            MovePanel(click_node.lock(), delta);
        }
    }
    // 窗口大小更改，更新面板大小
    int w, h;
    g4y::glview()->GetWindowSize(w, h);
    if(window_size != glm::ivec2(w, h)){
        window_size = glm::ivec2(w, h);
        root->rect = glm::ivec4(0, 0, h, w);
        UpdatePanel(root);
    }
}

bool PanelMgr::SwapPanel(std::shared_ptr<PanelNode> panel1, std::shared_ptr<PanelNode> panel2)
{
	return false;
}

bool PanelMgr::DelPanel(std::shared_ptr<PanelNode> panel)
{
	return false;
}

bool PanelMgr::MovePanel(std::shared_ptr<PanelNode> panel, int delta)
{
    panel->div_pos += delta;
    if(!CheckPos(panel)){
        panel->div_pos -= delta;
        return false;
    }
    CheckCopy(panel);
    if(!CheckMove(panel)){
        panel->div_pos -= delta;
        return false;
    }
    UpdatePanel(panel);
    return true;
}

bool PanelMgr::AddPanel(std::shared_ptr<PanelNode> p, PanelNode::PANEL_TYPE t, std::shared_ptr<PanelNode> l, std::shared_ptr<PanelNode> r)
{
    if(p == nullptr) p = root;
    assert(p->left.expired() && p->right.expired());
    p->type = t;
    if(t == PanelNode::PANEL_H){
        if(p->rect.w < 2 * width_min) return false;
    }else{
        if(p->rect.z < 2 * height_min) return false;
    }

    l->parent = p;
    r->parent = p;

    if(t == PanelNode::PANEL_H){
        p->div_pos = p->rect.w / 2;
        l->isleft = true;
        l->rect = glm::ivec4(p->rect.x, p->rect.y, p->rect.z, p->div_pos);

        r->isleft = false;
        r->rect = glm::ivec4(p->rect.x + p->div_pos, p->rect.y, p->rect.z, p->rect.w - p->div_pos);
    }else{
        p->div_pos = p->rect.z / 2;
        l->isleft = true;
        l->rect = glm::ivec4(p->rect.x, p->rect.y, p->div_pos, p->rect.w);

        r->isleft = false;
        r->rect = glm::ivec4(p->rect.x, p->rect.y + p->div_pos, p->rect.z - p->div_pos, p->rect.w);
    }
    
    p->left = l;
    p->right = r;
    CheckCopy(p);
    return true;
}

void PanelMgr::ModifyRect(std::shared_ptr<PanelNode> panel, bool split)
{
    auto node = panel;
    if(node->parent.expired()) return;
    auto p = node->parent.lock();
    glm::ivec4* prect;
    int* pdiv_pos;
    glm::ivec4* rect;
    int* div_pos;

    if(!split){
        prect = &p->rect2;
        pdiv_pos = &p->div_pos2;
        rect = &node->rect2;
        div_pos = &node->div_pos2;
    }else{
        prect = &p->rect;
        pdiv_pos = &p->div_pos;
        rect = &node->rect;
        div_pos = &node->div_pos;
    }

    float proportion;
    if(node->type == PanelNode::PANEL_H){
        proportion = *div_pos * 1.0f / rect->w;
    }else{
        proportion = *div_pos * 1.0f / rect->z;
    }

    if(node->isleft){
        if(p->type == PanelNode::PANEL_H)
            *rect = glm::ivec4((*prect).x, (*prect).y, (*prect).z, *pdiv_pos);
        else
            *rect = glm::ivec4((*prect).x, (*prect).y, *pdiv_pos, (*prect).w);
    }else{
        if(p->type == PanelNode::PANEL_H)
            *rect = glm::ivec4((*prect).x + *pdiv_pos, (*prect).y, (*prect).z, (*prect).w - *pdiv_pos);
        else
            *rect = glm::ivec4((*prect).x, (*prect).y + *pdiv_pos, (*prect).z - *pdiv_pos, (*prect).w);
    }
    *div_pos = (node->type == PanelNode::PANEL_H) ? ((*rect).w * proportion) : ((*rect).z * proportion);
}

void PanelMgr::UpdatePanel(std::shared_ptr<PanelNode> panel)
{
    if(panel == nullptr) return;

    // 修改面板大小
    ModifyRect(panel, true);

    UpdatePanel(panel->left.expired() ? nullptr : panel->left.lock());
    UpdatePanel(panel->right.expired() ? nullptr : panel->right.lock());

    // 回溯修正不符合的面板分割
    if(!panel->parent.expired()){
        auto p = panel->parent.lock();
        auto l = p->left.lock();
        auto r = p->right.lock();
        if(p->type == PanelNode::PANEL_H){
            if(p->div_pos < width_min){
                p->div_pos = width_min;
                l->rect.w = width_min;
                r->rect.x = p->div_pos;
                r->rect.w = p->rect.w - p->div_pos;
            }
            if(p->rect.w - p->div_pos < width_min){
                p->div_pos = p->rect.w - width_min;
                l->rect.w = p->div_pos;
                r->rect.x = p->div_pos;
                r->rect.w = width_min;
            }
        }else{
            if(p->div_pos < height_min){
                p->div_pos = height_min;
                l->rect.z = height_min;
                r->rect.y = p->div_pos;
                r->rect.z = p->rect.z - p->div_pos;
            }
            if(p->rect.z - p->div_pos < height_min){
                p->div_pos = p->rect.z - height_min;
                l->rect.z = p->div_pos;
                r->rect.y = p->div_pos;
                r->rect.z = height_min;
            }
        }
    }
}

bool PanelMgr::CheckPos(std::shared_ptr<PanelNode> panel)
{
    if(panel->type == PanelNode::PANEL_H)
        return (panel->div_pos >= width_min) && ((panel->rect.w - panel->div_pos >= width_min));
    else
        return (panel->div_pos >= height_min) && ((panel->rect.z - panel->div_pos >= height_min));
}

void PanelMgr::CheckCopy(std::shared_ptr<PanelNode> panel)
{
    if(panel == nullptr) return;
    panel->div_pos2 = panel->div_pos;
    panel->rect2 = panel->rect;
    CheckCopy(panel->left.expired() ? nullptr : panel->left.lock());
    CheckCopy(panel->right.expired() ? nullptr : panel->right.lock());
}

bool PanelMgr::CheckMove(std::shared_ptr<PanelNode> panel)
{
    if(panel == nullptr) return true;

    // 修改面板大小
    ModifyRect(panel, false);

    bool cl = CheckMove(panel->left.expired() ? nullptr : panel->left.lock());
    bool cr = CheckMove(panel->right.expired() ? nullptr : panel->right.lock());

    // 回溯检查该次移动是否合法
    if(!panel->parent.expired()){
        auto p = panel->parent.lock();
        if(p->type == PanelNode::PANEL_H){
            if(p->rect2.w < (width_min * 2)){
                return false;
            }
        }else{
            if(p->rect2.z  < (height_min * 2)){
                return false;
            }
        }
    }
    return (cl && cr);
}

void PanelMgr::Test()
{
    PanelMgr mgr;
    mgr.width_min = 5;
    mgr.height_min = 5;
    mgr.root->div_pos = 0;
    mgr.root->rect = glm::ivec4(0, 0, 40, 40);
    mgr.root->isleft = true;

    auto p1 = std::make_shared<PanelNode>();
    auto p2 = std::make_shared<PanelNode>();
    auto p3 = std::make_shared<PanelNode>();
    auto p4 = std::make_shared<PanelNode>();

    mgr.AddPanel(nullptr, PanelNode::PANEL_H, p1, p2);
    mgr.AddPanel(p2, PanelNode::PANEL_H, p3, p4);

    std::cout << mgr.MovePanel(mgr.root, -16) << std::endl;
}

