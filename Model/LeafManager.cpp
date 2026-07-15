#include "LeafManager.h"

#include "MathUtils.h"

#include <algorithm>

LeafManager::LeafManager(float screenWidth,float scrollSpeed,float initialPlatformX,float initialPlatformY,float minimumLeafY,float maximumLeafY)
    : screenWidth(screenWidth),
      scrollSpeed(scrollSpeed),
      minimumLeafY(minimumLeafY),
      maximumLeafY(maximumLeafY),
      initialPlatformX(initialPlatformX),
      initialPlatformY(initialPlatformY),
      lastSpawnY(initialPlatformY),
      randomEngine(std::random_device{}())
{ createInitialLeaves();}

void LeafManager::createInitialLeaves()
{
    //在玩家起点右侧生成6片叶子
    leaves.emplace_back(LeafType::Normal, initialPlatformX, initialPlatformY);
    leaves.emplace_back(LeafType::Normal, initialPlatformX + 190.0f, initialPlatformY - 36.0f);
    leaves.emplace_back(LeafType::Normal, initialPlatformX + 380.0f, initialPlatformY - 68.0f);
    leaves.emplace_back(LeafType::Dark, initialPlatformX + 580.0f, initialPlatformY - 26.0f);
    leaves.emplace_back(LeafType::Normal, initialPlatformX + 790.0f, initialPlatformY - 74.0f);
    leaves.emplace_back(LeafType::Golden, initialPlatformX + 1010.0f, initialPlatformY - 38.0f);
    //记录最后一片叶子的坐标
    lastSpawnY = initialPlatformY - 38.0f;
}

void LeafManager::update(float dt)
{
    if (dt <= 0.0f)
        return;

    for (Leaf& leaf : leaves)
        leaf.update(dt, scrollSpeed);

    for (auto it = leaves.begin(); it != leaves.end(); )
    {
        if (it->isExpired())
            it = leaves.erase(it); 
        else
            ++it;
    }//删除死亡叶子

    spawnTimer -= dt;

    if (spawnTimer <= 0.0f) //每隔一段时间生成一片随即叶子
        spawnLeaf();
}

void LeafManager::spawnLeaf()
{
    std::uniform_real_distribution<float> intervalDistribution(1.3f, 2.0f);//生成间隔
    std::uniform_real_distribution<float> verticalDistribution(-110.0f, 150.0f); //y轴偏移

    //在上一片叶子 Y 坐标基础上增加随机偏移，然后用clampFloat限制在全局最小/最大范围内
    lastSpawnY = clampFloat(lastSpawnY + verticalDistribution(randomEngine),minimumLeafY,maximumLeafY);

    leaves.emplace_back(createRandomLeafType(),screenWidth + 140.0f,lastSpawnY); //在可视区域右侧外部生成，然后向左滚动进入画面
    spawnTimer = intervalDistribution(randomEngine);//重置计时，设定下一次生成的时间间隔（1.3~2.0 秒），形成动态变化
}

LeafType LeafManager::createRandomLeafType()
{
    std::uniform_int_distribution<int> probabilityDistribution(1, 100);

    const int probability = probabilityDistribution(randomEngine); //生成随机数采样生成概率

    if (probability <= 15) return LeafType::Golden; //金色叶子概率 15%
    if (probability <= 40) return LeafType::Dark; //深色叶子概率 25%
    return LeafType::Normal; //普通叶子概率 60%
}

const std::vector<Leaf>& LeafManager::getLeaves() const noexcept
{
    return leaves;
}

std::vector<Leaf>& LeafManager::getMutableLeaves() noexcept
{
    return leaves;
}
