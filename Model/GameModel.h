#pragma once

#include "CollisionDetector.h"
#include "LeafManager.h"
#include "Player.h"

#include <vector>

enum class GameEventType //Model 层能够向上层报告的事件类型
{
    DarkLeafTriggered,
    GoldenBoostStarted,
    ScoreChanged,
    GameOver
};

struct GameEvent
{
    GameEventType type;
    int value{0}; //分数
};

class GameModel
{
public:
    GameModel(float screenWidth,float screenHeight);

    void update(float dt);
    //玩家控制命令接口
    void jump() noexcept;
    void moveLeft() noexcept;
    void moveRight() noexcept;
    void stopMoving() noexcept;
    void setGrabRequested(bool requested) noexcept;

    const Player& getPlayer() const noexcept;
    const std::vector<Leaf>& getLeaves() const noexcept;
    int getScore() const noexcept;
    bool isGameOver() const noexcept;
    std::vector<GameEvent> takeEvents();

private:
    static constexpr float InitialPlayerX = 140.0f;
    static constexpr float InitialPlatformX = 100.0f;
    static constexpr float InitialPlatformBottomMargin = 150.0f;
    static constexpr float MinimumLeafY = 180.0f;
    static constexpr float MaximumLeafBottomMargin =110.0f;
    static constexpr float LeafScrollSpeed = 95.0f;
    static constexpr float GoldenBoostDuration = 0.8f;
    static constexpr float GoldenCarrySpeed = 320.0f;
    static constexpr float GoldenCarryDistance = 240.0f;

    float screenWidth;
    float screenHeight;

    Player player;
    LeafManager leafManager;
    CollisionDetector collisionDetector;

    float survivalTime{0.0f};
    int survivalScore{0};
    float goldenBonusTime{0.0f};
    int goldenBonusScore{0};
    float goldenCarryDistanceRemaining{0.0f};
    bool gameOver{false};

    std::vector<GameEvent> events;

    void resolveLeafInteractions(float dt); //碰撞处理逻辑
    void applyLeafEffect(LeafEffect effect); //根据叶子返回的效果码，执行对应的游戏规则
    void updateGoldenCarry(Leaf& leaf, float dt);
    void updateScore(float dt);
    void endGame();
};
