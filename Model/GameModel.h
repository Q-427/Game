#pragma once

#include "../Common/INotifyPropertyChanged.h"
#include "CollisionDetector.h"
#include "LeafManager.h"
#include "Player.h"

#include <vector>

namespace GameModelProperty
{
    inline constexpr const char* Score = "Score";
    inline constexpr const char* GameOver = "GameOver";
    inline constexpr const char* GoldenBoost = "GoldenBoost";
    inline constexpr const char* DarkLeaf = "DarkLeaf";
    inline constexpr const char* PlayerState = "PlayerState";
    inline constexpr const char* Leaves = "Leaves";
    inline constexpr const char* Reset = "Reset";
}

class GameModel : public INotifyPropertyChanged
{
public:
    GameModel(float screenWidth,float screenHeight);

    void update(float dt);
    void reset(); //重新开始
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

private:
    static constexpr float InitialPlayerX = 140.0f;
    static constexpr float InitialPlatformX = 100.0f;
    static constexpr float InitialPlatformBottomMargin = 150.0f;
    static constexpr float MinimumLeafY = 180.0f;
    static constexpr float MaximumLeafBottomMargin =110.0f;
    static constexpr float LeafScrollSpeed = 200.0f;
    static constexpr float GoldenEffectDuration = 1.0f;
    static constexpr float GoldenCarrySpeed = 320.0f;
    static constexpr float GoldenCarryDistance = GoldenCarrySpeed * GoldenEffectDuration;

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

    void resolveLeafInteractions(float dt); //碰撞处理逻辑
    void applyLeafEffect(LeafEffect effect); //根据叶子返回的效果码，执行对应的游戏规则
    void updateGoldenCarry(Leaf& leaf, float dt);
    void updateScore(float dt);
    void endGame();
};
