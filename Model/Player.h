#pragma once

class CollisionDetector;
class GameModel;
struct Rect;

class Player
{
public:
    //玩家碰撞尺寸
    static constexpr float CollisionWidth = 36.0f;
    static constexpr float CollisionHeight = 58.0f;

    explicit Player(float initialX,float initialY) noexcept;

    //暴露各个物理参数
    float getX() const noexcept;
    float getY() const noexcept;
    float getVelocityX() const noexcept;
    float getVelocityY() const noexcept;
    float getPreviousBottom() const noexcept;

    bool isGrounded() const noexcept; //是否站在叶子上
    bool isGrabRequested() const noexcept; //是否在请求抓取
    bool isHanging() const noexcept; //是否抓取成功后悬停
    bool hasGoldenBoost() const noexcept; //是否处在金色叶子触发的快速状态

private:
    static constexpr float Gravity = 2400.0f; //重力加速度
    static constexpr float MoveSpeed = 190.0f; //普通移动速度
    static constexpr float GoldenMoveSpeed = 320.0f; //碰到金叶子后的速度
    static constexpr float JumpSpeed = 800.0f; //起跳的初速度
    static constexpr float MaxFallSpeed = 1200.0f; //下落速度上线，防止穿透物体
    static constexpr int MaxJumpCount = 2;

    float xPosition;
    float yPosition; //玩家顶部坐标（y轴向下为正）
    float previousY; //前一帧的y坐标，用于碰撞判定
    float velocityX{0.0f};
    float velocityY{0.0f};
    float goldenBoostRemaining{0.0f}; //金叶子冲刺剩余时间
    int jumpCount{0};

    bool grounded{false};
    bool grabRequested{false};
    bool hanging{false};

    void update(float dt) noexcept;  //更新下一帧的物理状态
    void moveLeft() noexcept;
    void moveRight() noexcept;
    void stopMoving() noexcept;
    void jump() noexcept;
    void setGrabRequested(bool requested) noexcept;
    void landOn(float platformTop) noexcept; //玩家踩到叶子后调用
    void setHanging(bool value) noexcept; //抓取成功时调用此方法
    void activateGoldenBoost(float duration) noexcept;
    void moveHorizontal(float distance) noexcept;
    void constrainX(float minimum, float maximum) noexcept; //将玩家坐标约束在合法范围内，防止超出屏幕边界
    Rect getBounds() const noexcept; //获得实时的碰撞箱

    friend class CollisionDetector;
    friend class GameModel;
};
