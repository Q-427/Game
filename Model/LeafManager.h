#pragma once

#include "Leaf.h"

#include <random>
#include <vector>

class GameModel;

class LeafManager
{
public:
    LeafManager(
        float screenWidth,
        float scrollSpeed,
        float initialPlatformX,//玩家起点下方第一片初始叶子的位置
        float initialPlatformY,
        float minimumLeafY,//叶子生成时Y坐标的随机范围
        float maximumLeafY);

    void update(float dt);

    const std::vector<Leaf>& getLeaves() const noexcept; //不可修改叶子状态

private:
    float screenWidth;
    float scrollSpeed;
    float minimumLeafY;
    float maximumLeafY;

    float initialPlatformX;
    float initialPlatformY;
    float spawnTimer{1.5f}; //生成倒计时
    float lastSpawnY; //上一片叶子的Y坐标

    std::vector<Leaf> leaves; //存储所有活跃叶子的动态数组
    std::mt19937 randomEngine; //Mersenne Twister 随机数引擎，用于生成随机类型和随机高度

    void createInitialLeaves(); //生成起始平台（initialPlatformX/Y 位置的叶子）以及头几片叶子
    void spawnLeaf(); //生成一片新叶子
    LeafType createRandomLeafType();

    std::vector<Leaf>& getMutableLeaves() noexcept; //仅允许GameModel获取写入权限，防止外部模块绕过GameModel直接修改叶子

    friend class GameModel;
};