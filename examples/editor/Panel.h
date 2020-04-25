#ifndef __PANEL_H__
#define __PANEL_H__
#include "G4Y.h"

class PanelMgr;
class PanelNode : public GCom, public std::enable_shared_from_this<PanelNode>
{
    G_COM
    friend class PanelMgr;
public:
    enum PANEL_TYPE{ PANEL_H, PANEL_V };

    PanelNode();

    void SetWindowOpt();
    std::shared_ptr<PanelNode> CreateInitEmptyPanel();

    virtual void Start() override;

    virtual void Update() override;    

	void AddPanelNode(std::shared_ptr<PanelNode> n);
    
    bool       isleft;

    PANEL_TYPE type;
    
    // x, y, z --> height, w --> width
    glm::ivec4 rect; 
    int        div_pos = 0;

    glm::ivec4 rect2; 
    int        div_pos2 = 0;
    
    ImGuiWindowFlags window_flags = 0;
    int panel_idx = -1;
    int panel_num = 0;
    static int s_panel_cnt;

    std::weak_ptr<PanelNode> parent;
    std::weak_ptr<PanelNode> left;
    std::weak_ptr<PanelNode> right;
    std::weak_ptr<PanelMgr>  panel_mgr;
};

class Panel : public GCom
{
    G_COM
public:
    virtual std::shared_ptr<Panel> Clone() = 0;

    virtual void Resize(glm::ivec4) = 0;

    virtual std::string PanelName() = 0;
};

#endif