#ifndef __PANELMGR_H__
#define __PANELMGR_H__
#include "G4Y.h"
#include "Panel.h"

class PanelMgr : public GCom, public std::enable_shared_from_this<PanelMgr>
{
    friend class PanelNode;
    friend class PanelTool;
public:
    PanelMgr();

    virtual void Start() override;

    virtual void Update() override;

    virtual std::string ComName() { return "PanelMgr"; }

    // 注册渲染的面板
    template<typename T>
    bool RegPanel(std::shared_ptr<T> p)
    {
        bool ret = true;
        auto panel = std::dynamic_pointer_cast<Panel>(p);
        if(panel) act_panels.push_back(std::make_pair(false, panel));
        else return false;
        return true;
    }

    // 删除面板
    void UnregPanel(std::shared_ptr<PanelNode>);

    // 仅用于测试
    static void Test();

private:
    bool MovePanel(std::shared_ptr<PanelNode> panel, int div_pos);

    bool AddPanel(std::shared_ptr<PanelNode> p,  PanelNode::PANEL_TYPE t, std::shared_ptr<PanelNode> l, std::shared_ptr<PanelNode> r);

    bool SwapPanel(std::shared_ptr<PanelNode> panel1, std::shared_ptr<PanelNode> panel2);

    bool DelPanel(std::shared_ptr<PanelNode> panel);
    
    std::shared_ptr<PanelNode> IsPosInSplitLine(std::shared_ptr<PanelNode> panel, glm::ivec2 pos);
    bool PosInRect(glm::ivec2, glm::ivec4);
    void ModifyRect(std::shared_ptr<PanelNode> panel, bool split = true);
    bool CheckPos(std::shared_ptr<PanelNode> panel);
    void CheckCopy(std::shared_ptr<PanelNode> panel);
    bool CheckMove(std::shared_ptr<PanelNode> panel);
    void UpdatePanel(std::shared_ptr<PanelNode> panel);

    std::shared_ptr<PanelNode> GetLeftLeaf(std::shared_ptr<PanelNode>);
private:
    std::shared_ptr<PanelNode> root;
    std::weak_ptr<PanelNode> click_node;
    int width_min = 50;
    int height_min = 50;
    glm::ivec2 window_size;

    std::vector<std::pair<bool,std::weak_ptr<Panel>>> act_panels;
};

#endif