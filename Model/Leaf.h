#pragma once

#include "../Common/MathUtils.h"

enum class LeafType
{
    Normal,
    Dark,
    Golden
};

enum class LeafEffect
{
    None,
    DarkBreakStarted,//通知 GameModel 该深色叶子开始碎裂倒计时
    GoldenBoostActivated //通知 GameModel 给玩家施加金叶子增益。
};

class GameModel;
class LeafManager;

class Leaf
{
public:
    //叶子碰撞箱尺寸
    static constexpr float CollisionWidth = 112.0f;
    static constexpr float CollisionHeight = 22.0f;

    Leaf(LeafType initialType,float initialX,float initialY) noexcept;

    LeafType getType() const noexcept;

    float getX() const noexcept;
    float getY() const noexcept;

    bool isBreaking() const noexcept; //叶子是否进入碎裂倒计时
    bool isExpired() const noexcept; //叶子是否死亡（移除屏幕或被击碎）
    bool hasTriggeredEffect() const noexcept; //确保“金叶子加速”等效果只触发一次

    Rect getBounds() const noexcept; //返回当前叶片的碰撞箱，用于踩踏判定
    Rect getStemBounds() const noexcept; //返回茎部的碰撞箱，用于抓取判定

private:
    LeafType type;
    float xPosition;
    float yPosition;
    float breakTimeRemaining{0.0f};

    bool effectTriggered{false};
    bool breaking{false};
    bool expired{false};

    void update(float dt,float scrollSpeed) noexcept; //叶子自身的帧更新

    LeafEffect onPlayerLanded() noexcept; //当玩家踩中这片叶子时，GameModel 调用此方法

    friend class GameModel;
    friend class LeafManager;
};