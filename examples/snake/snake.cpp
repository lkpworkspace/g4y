#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include <iostream>
#include <random>
#include <list>
#include <algorithm>
#include <time.h>

#include "G4Y.h"
#include "RoamScript.h"
#include "CameraScript.h"

enum NODE_TYPE { NONE, FOOD, BODY };
enum DIRECTION { LEFT = -1, RIGHT = 1, UP = 2, DOWN = -2};
enum GAME_STATE{ UNKNOWN_STATE, UNINIT, GAMESTART, GAMEING, GAME_WIN, GAME_LOST };

class NodeScript : public GCom
{
    G_COM
public:
    void SetNodeType(NODE_TYPE t){
        this->type = t;
    }

    NODE_TYPE GetType(){ return type; }

    virtual void Start() override
    {
        m_transform = GetCom<GTransform>();
    }

    virtual void Update() override
    {
        float delta_time = GWorld::GetDeltaTime();
        float speed = 90.0f;
        yaw += speed * delta_time;
        if(type == BODY){
            m_transform.lock()->SetRotation(glm::vec3(0, 0, 0));
        }else{
            m_transform.lock()->SetRotation(glm::vec3(0, yaw, 0));
        }
    }

    float yaw = 0.0f;
    NODE_TYPE type = FOOD;
    std::weak_ptr<GTransform> m_transform;
};


class SnakeScript : public GCom
{
    G_COM
public:
    SnakeScript(std::string title) :
        title(title),
        row_cnt(15),
        col_cnt(15)
    {}

    void GameInit()
    {
        if(state != UNINIT) return;
        // 填充棋盘
        for(int i = 1; i <= row_cnt; ++i){
            std::vector<NodeInfo> vec;
            for(int j = 1; j <= col_cnt; ++j){
                NodeInfo info;
                info.idx[0] = i - 1;
                info.idx[1] = j - 1;
                info.pos = glm::vec3(i * offset, j * offset, 0);
                info.obj.reset();
                vec.push_back(info);
                m_empty_chess_vec.push_back(std::make_pair(i - 1, j - 1));
            }
            m_chessboard.push_back(vec);
        }
        // 生成棋盘边框
        for(int i = 0; i <= row_cnt + 1; ++i){
            for(int j = 0; j <= col_cnt + 1; ++j){
                if(i == 0 || j == 0 || i == row_cnt + 1 || j == col_cnt + 1){
                    auto o = CreateNode(glm::vec4(1, 1, 1, 0.5));
                    auto script = o->GetCom<NodeScript>();
                    script->SetNodeType(NONE);
                    o->GetCom<GTransform>()->SetPosition(glm::vec3(i * offset, j * offset, 0));
                }
            }
        }
        state = GAMESTART;
    }

    void Reset()
    {
        if(state == UNINIT) return;

        m_chessboard.clear();
        m_empty_chess_vec.clear();
        m_used_chess_vec.clear();
        m_snake.clear();
        for(const auto& o : m_all_objs){
            assert(!o.expired());
            GObj::Destroy(o.lock());
        }
        m_all_objs.clear();
        state = UNINIT;
    }

    glm::vec4 RandomVec4()
    {
        static std::default_random_engine random(time(NULL));
        static std::uniform_real_distribution<float> dis(0.0, 1.0);
        return glm::vec4(dis(random), dis(random), dis(random), 0.5f);
    }

    std::shared_ptr<GObj> CreateNode(glm::vec4 color)
    {
        auto o = std::make_shared<GObj>();
        
        o->AddDefaultComs();
        auto node = std::make_shared<GCube>();
        auto script = std::make_shared<NodeScript>();
        node->SetColor(color);
        o->AddCom(node);
        o->AddCom(script);
        g4y::curscene()->AddChild(o);

        m_all_objs.push_back(o);
        return o;
    }

    bool RandomNode(std::shared_ptr<GObj> o, NODE_TYPE t)
    {
        if(m_empty_chess_vec.empty()) return false;
        
        std::default_random_engine dre;
        std::random_device rd{};
        std::seed_seq seeds{ rd(), rd() };
        dre.seed(seeds);
        std::shuffle(m_empty_chess_vec.begin(), m_empty_chess_vec.end(), dre);
        auto idx = m_empty_chess_vec.back();
        m_empty_chess_vec.pop_back();
        m_used_chess_vec.push_back(idx);
        
        auto script = o->GetCom<NodeScript>();
        script->SetNodeType(t);
        o->GetCom<GTransform>()->SetPosition(m_chessboard[idx.first][idx.second].pos);
        m_chessboard[idx.first][idx.second].obj = o;
        return true;
    }

    void InsertNode(std::shared_ptr<GObj> o, NODE_TYPE t, int i, int j)
    {
        auto script = o->GetCom<NodeScript>();
        script->SetNodeType(t);
        o->GetCom<GTransform>()->SetPosition(m_chessboard[i][j].pos);

        auto it = std::remove(m_empty_chess_vec.begin(), m_empty_chess_vec.end(), std::make_pair(i, j));
        m_empty_chess_vec.erase(it, m_empty_chess_vec.end());
        m_used_chess_vec.push_back(std::make_pair(i, j));

        m_chessboard[i][j].obj = o;
    }

    std::shared_ptr<GObj> RemoveNode(int i, int j)
    {
        auto obj = m_chessboard[i][j].obj.lock();
        m_chessboard[i][j].obj.reset();
        auto it = std::remove(m_used_chess_vec.begin(), m_used_chess_vec.end(), std::make_pair(i, j));
        m_used_chess_vec.erase(it, m_used_chess_vec.end());
        m_empty_chess_vec.push_back(std::make_pair(i, j));
        
        return obj;
    }

    NODE_TYPE GetType(int i, int j)
    {
        assert(!m_chessboard.empty());
        if(m_chessboard[i][j].obj.expired()) return NONE;
        auto obj = m_chessboard[i][j].obj.lock();
        auto node_com = obj->GetCom<NodeScript>();
        return node_com->GetType();
    }

    void EatFood(int i, int j)
    {
        // 设置食物变成蛇的身体，添加至蛇的链表头
        auto obj = m_chessboard[i][j].obj.lock();
        auto node_com = obj->GetCom<NodeScript>();
        node_com->SetNodeType(BODY);
        m_snake.push_front(std::make_pair(i, j));
    }

    void Move(int i, int j)
    {
        #if 0
        // 将蛇的尾部节点设置到蛇头
        auto n = RemoveNode(m_snake.back().first, m_snake.back().second);
        InsertNode(n, BODY, i, j);
        m_snake.push_front(std::make_pair(i, j));
        m_snake.pop_back();
        #else
        // 将所有节点向前移动
        int fwd_i = i, fwd_j = j;
        for(auto begin = m_snake.begin(); begin != m_snake.end(); ++begin){
            int tmp_i = (*begin).first;
            int tmp_j = (*begin).second;
            *begin = std::make_pair(fwd_i, fwd_j);
            auto n = RemoveNode(tmp_i, tmp_j);
            InsertNode(n, BODY, fwd_i, fwd_j);
            fwd_i = tmp_i;
            fwd_j = tmp_j;
        }
        #endif
    }

    void GameStart()
    {
        if(state != GAMESTART) return;
        // 创建一个节点当做蛇头
        auto o = CreateNode(RandomVec4());
        InsertNode(o, BODY, row_cnt / 2, col_cnt / 2);
        m_snake.push_back(std::make_pair(row_cnt / 2, col_cnt / 2));

        // 创建一个节点做豆豆
        auto f = CreateNode(RandomVec4());
        RandomNode(f, FOOD);
        // 设置游戏开始
        state = GAMEING;
    }

    virtual void Start() override
    {
        GameInit();
    }

    virtual void Update() override
    {
        ImGui::Begin(title.c_str());
        ImGui::SliderFloat("Move Speed", &internal, 0.1f, 2.0f);
        if(state == UNINIT && ImGui::Button("Game Init")){
            GameInit();
        }
        if(state == GAMESTART && ImGui::Button("Game Start")){
            GameStart();
        }
        if(state != UNINIT && ImGui::Button("Game Reset")){
            Reset();
        }
        if(state == GAMESTART && ImGui::Button("Game Test")){
            for(int i = 1; i <= row_cnt; ++i){
                for(int j = 1; j <= col_cnt; ++j){
                    auto node = CreateNode(RandomVec4());
                    InsertNode(node, FOOD, i - 1, j - 1);
                }
            }
            state = UNKNOWN_STATE;
        }
        if(state == GAMEING){
            if(m_empty_chess_vec.empty()){
                state = GAME_WIN;
            }else{
                float delta_time = GWorld::GetDeltaTime();
                cost += delta_time;
                if(cost > internal)
                {
                    cost = 0;
                    // 启动游戏循环
                    // 每隔1秒钟蛇移动一次
                    auto i = m_snake.front().first;
                    auto j = m_snake.front().second;
                    if(last_dir == -1 * dir){
                        dir = last_dir;
                    }
                    switch (dir)
                    {
                    case LEFT: i -= 1; break;
                    case RIGHT: i += 1; break;
                    case UP: j += 1; break;
                    case DOWN: j -= 1; break;
                    }
                    if(i < 0 || i >= row_cnt || j < 0 || j >= col_cnt){
                        state = GAME_LOST;
                    }else{
                        auto type = GetType(i, j);
                        if(type == NONE){
                            Move(i, j);
                        }
                        if(type == FOOD){
                            EatFood(i, j);
                            auto node = CreateNode(RandomVec4());
                            if(!RandomNode(node, FOOD)){
                                state = GAME_WIN;    
                            }
                        }
                        if(type == BODY){
                            state = GAME_LOST;
                        }
                    }
                    last_dir = dir;
                }
            }
        }
        if(state == GAME_LOST){
            // 游戏结束
            ImGui::Text("You Failed");
        }
        if(state == GAME_WIN){
            // 游戏结束
            ImGui::Text("You Win");
        }
        // UP 0x109, DOWN 0x108, LEFT 0x107, RIGHT 0x106
        if(ImGui::IsKeyPressed(0x109)) dir = UP;
        if(ImGui::IsKeyPressed(0x108)) dir = DOWN;
        if(ImGui::IsKeyPressed(0x107)) dir = LEFT;
        if(ImGui::IsKeyPressed(0x106)) dir = RIGHT;
        
        ImGui::End();
    }

    struct NodeInfo{
        int idx[2];
        glm::vec3 pos;
        std::weak_ptr<GObj> obj;

        NodeInfo() :
            pos(0,0,0)
        {
            idx[0] = 0;
            idx[1] = 0;
        }
    };

    int offset = 2;
    int row_cnt, col_cnt;
    
    float internal = 0.2f;
    float cost = 0.0f;
    std::string title;
    GAME_STATE state = UNINIT;
    DIRECTION  dir = UP;
    DIRECTION  last_dir = UP;
    std::vector<std::pair<int, int>>   m_empty_chess_vec;
    std::vector<std::pair<int, int>>   m_used_chess_vec;
    std::list<std::pair<int, int>>     m_snake;
    std::vector<std::vector<NodeInfo>> m_chessboard;
    std::vector<std::weak_ptr<GObj>>   m_all_objs;
};

void build_scene(std::shared_ptr<GScene> s)
{
    auto  camera = std::make_shared<GObj>();
    auto  grid = std::make_shared<GObj>();

    camera->SetTag("GCamera");
    camera->AddDefaultComs();
    camera->AddCom(std::make_shared<GCamera>());
    camera->AddCom(std::make_shared<RoamScript>("Camera roam"));
    camera->AddCom(std::make_shared<SnakeScript>("Snake"));
    camera->AddCom(std::make_shared<CameraScript>());
    // camera->AddCom(std::make_shared<GSkybox>(
    //     std::vector<std::string>({
    //         "/home/lkp/projs/gfy/build/skybox/right.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/left.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/top.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/bottom.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/front.jpg",
    //         "/home/lkp/projs/gfy/build/skybox/back.jpg",
    //     })
    // ));

    grid->AddDefaultComs();
    grid->AddCom(std::make_shared<GGrid>(-100, 100, 1));
 
    s->AddChild(camera);
    s->AddChild(grid);
}

int main(int argc, char** argv)
{
    const auto w = GWorld::Instance();
    std::shared_ptr<GScene> s = std::make_shared<GScene>();

    w->SetScene(s);

    build_scene(s);

    return w->Run();
}
