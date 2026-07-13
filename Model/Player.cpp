#include "Player.h"
#include "MathUtils.h"

#include <algorithm>

Player::Player(float initialX,float initialY) noexcept
    : xPosition(initialX), yPosition(initialY), previousY(initialY)
{
}

void Player::update(float dt) noexcept
{
    previousY = yPosition;
    goldenBoostRemaining = std::max(0.0f, goldenBoostRemaining - dt);

    if(hanging)
        velocityY = 0.0f;
    else
        velocityY = std::min(velocityY + Gravity * dt,MaxFallSpeed);

    xPosition += velocityX * dt;
    yPosition += velocityY * dt;

    if(velocityY != 0.0f)
        grounded = false;

}

void Player::moveLeft() noexcept
{
    velocityX = hasGoldenBoost() ? -GoldenMoveSpeed : -MoveSpeed;
}

void Player::moveRight() noexcept
{
    velocityX = hasGoldenBoost() ? GoldenMoveSpeed : MoveSpeed;
}

void Player::stopMoving() noexcept
{
    velocityX = 0.0f;
}

void Player::jump() noexcept
{
    if (jumpCount >= MaxJumpCount)
    {
        return;
    }

    ++jumpCount;
    velocityY = -JumpSpeed;
    grounded = false;
    hanging = false;
    grabRequested = false;
}

void Player::setGrabRequested(bool requested) noexcept
{
    grabRequested = requested;

    if (!requested)
        hanging = false; //松开Z键时自动脱离悬挂状态
}

void Player::landOn(float platformTop) noexcept
{
    yPosition = platformTop - CollisionHeight;
    velocityY = 0.0f;
    grounded = true;
    hanging = false;
    jumpCount = 0;
}

void Player::setHanging(bool value) noexcept
{
    hanging = value;

    if (hanging)
    {
        velocityY = 0.0f;
        grounded = false;
    }
}

void Player::activateGoldenBoost(float duration) noexcept
{
    goldenBoostRemaining = std::max(goldenBoostRemaining, duration);
    velocityX = 0.0f;
}

void Player::moveHorizontal(float distance) noexcept { xPosition += distance; }

void Player::constrainX(float minimum, float maximum) noexcept
{
    xPosition = clampFloat(xPosition, minimum, maximum);
}

float Player::getX() const noexcept { return xPosition; }
float Player::getY() const noexcept { return yPosition; }
float Player::getVelocityX() const noexcept { return velocityX; }
float Player::getVelocityY() const noexcept { return velocityY; }

float Player::getPreviousBottom() const noexcept{return previousY + CollisionHeight; } //获得玩家脚底的坐标

bool Player::isGrounded() const noexcept { return grounded; }
bool Player::isGrabRequested() const noexcept { return grabRequested; }
bool Player::isHanging() const noexcept { return hanging; }
bool Player::hasGoldenBoost() const noexcept{ return goldenBoostRemaining > 0.0f; }

Rect Player::getBounds() const noexcept
{
    return {
        xPosition,
        yPosition,
        CollisionWidth,
        CollisionHeight
    };
} //将当前坐标和固定尺寸打包成 Rect 结构体，供 GameModel 进行 intersects 碰撞检测
