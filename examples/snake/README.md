## 3D世界中的2D贪吃蛇

### 设计思路
一个vec保存没有使用过的格子的索引  --> m_empty_chess_vec
一个vec保存使用过的格子的索引     --> m_used_chess_vec
一个链表保存蛇的索引             --> m_snake

索引节点信息
{
    /* 索引下标 */
    int idx[2];
    /* 界面中的位置 */
    glm::vec3 pos;
    /* 格子中的节点，没有则为空 */
    std::weak_ptr<GObj> obj;
}

创建一个豆豆：
    从m_empty_chess_vec中删除
    添加至m_used_chess_vec

删除一个豆豆：
    从m_used_chess_vec中删除
    添加至m_empty_chess_vec

蛇吃到豆豆：
    豆豆加入到蛇链表头， 更改该节点属性
    再生成一个豆豆

蛇移动：
    四个方向(x, -x, y, -y)
    初始从中间开始向y方向移动
    如果为y或-y方向，则只有向 x, -x 方向移动
    如果为x或-x方向，则只有向 y, -y 方向移动
    将链表尾部的节点移动到链表头

判输：
    越界
    撞到自己的身体

判赢：
    m_empty_chess_vec为空
    随机豆豆失败


附加效果：
    - 豆豆使用旋转效果
    - 蛇身体不旋转

### 操作
1. w,a,s,d,space,ctrl按键控制镜头
2. 按钮
    - “Game Init” 初始化游戏按钮
    - “Game Start” 游戏开始
    - “Game Reset” 游戏复位
    - “Game Test” 测试使用

