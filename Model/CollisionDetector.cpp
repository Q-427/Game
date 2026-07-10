#include "CollisionDetector.h"

bool CollisionDetector::isLanding(const Player& player,const Leaf& leaf) const noexcept
{
    if (leaf.isExpired())
        return false;

    // 负 Y 速度表示玩家正在向上运动。
    if (player.getVelocityY() < 0.0f)
        return false;

    const Rect playerBounds =player.getBounds();
    const Rect leafBounds =leaf.getBounds();

    const bool horizontalOverlap = playerBounds.right() > leafBounds.x && playerBounds.x < leafBounds.right(); //水平重叠

    const bool crossedLeafTop = player.getPreviousBottom() <= leafBounds.y + 4.0f && playerBounds.bottom() >= leafBounds.y;
    //上一帧脚底在叶子顶部上方且当前帧脚底已经低于或恰好等于叶子顶部
    return horizontalOverlap && crossedLeafTop;
}

bool CollisionDetector::canGrab(const Player& player,const Leaf& leaf) const noexcept
{
    if (leaf.isExpired())
        return false;

    constexpr float grabRange = 18.0f; //创建一个以玩家碰撞箱为中心的扩大矩形，左右上下各扩展 18 像素
    const Rect playerBounds = player.getBounds();

    const Rect grabArea{
        playerBounds.x - grabRange,
        playerBounds.y - grabRange,
        playerBounds.width + grabRange * 2.0f,
        playerBounds.height + grabRange * 2.0f
    };

    return grabArea.intersects(leaf.getStemBounds()); //检测扩展区域与叶子茎部碰撞箱是否相交
}