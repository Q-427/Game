#include "Leaf.h"

Leaf::Leaf(LeafType initialType,float initialX,float initialY) noexcept: type(initialType),xPosition(initialX),yPosition(initialY){}

void Leaf::update(float dt,float scrollSpeed) noexcept
{
    xPosition -= scrollSpeed * dt;

    if (breaking)
    {
        breakTimeRemaining -= dt;

        if (breakTimeRemaining <= 0.0f)
            expired = true;
    }

    if (xPosition + CollisionWidth < 0.0f) //叶子右边缘移出屏幕
        expired = true;
}

LeafEffect Leaf::onPlayerLanded() noexcept
{
    if (effectTriggered)
        return LeafEffect::None; //防止重复触发

    switch (type)
    {
    case LeafType::Dark:
        effectTriggered = true;
        breaking = true;
        breakTimeRemaining = 0.45f;
        return LeafEffect::DarkBreakStarted;

    case LeafType::Golden:
        effectTriggered = true;
        return LeafEffect::GoldenBoostActivated;

    case LeafType::Normal:
    default:
        return LeafEffect::None;
    }
}

LeafType Leaf::getType() const noexcept { return type; }
float Leaf::getX() const noexcept { return xPosition; }
float Leaf::getY() const noexcept { return yPosition; }
bool Leaf::isBreaking() const noexcept { return breaking; }
bool Leaf::isExpired() const noexcept { return expired; }
bool Leaf::hasTriggeredEffect() const noexcept { return effectTriggered; }

Rect Leaf::getBounds() const noexcept
{
    return {
        xPosition,//左上角坐标
        yPosition,
        CollisionWidth,
        CollisionHeight
    };
}

Rect Leaf::getStemBounds() const noexcept
{
    constexpr float stemWidth = 12.0f;
    constexpr float stemHeight = CollisionHeight * 2.4f;

    const float stemX = xPosition + CollisionWidth * 0.5f - stemWidth * 0.5f; //叶茎左上角

    const float stemY = yPosition + CollisionHeight * 0.75f;

    return {
        stemX,
        stemY,
        stemWidth,
        stemHeight
    };
}
