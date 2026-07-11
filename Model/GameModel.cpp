#include "GameModel.h"

#include <algorithm>

GameModel::GameModel(float screenWidth,float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      player(InitialPlayerX,0.0f),
      leafManager(screenWidth,LeafScrollSpeed,InitialPlatformX,screenHeight - InitialPlatformBottomMargin,MinimumLeafY,screenHeight - MaximumLeafBottomMargin)
{
    const float initialPlatformY = screenHeight - InitialPlatformBottomMargin;
    player.landOn(initialPlatformY); //强制玩家在初始平台上
}

void GameModel::update(float dt)
{
    if (gameOver || dt <= 0.0f)
        return;
    player.update(dt);
    leafManager.update(dt);
    resolveLeafInteractions(dt);
    player.constrainX(0.0f,screenWidth - Player::CollisionWidth);
    updateScore(dt);

    if (player.getY() > screenHeight)
        endGame();//玩家掉出屏幕底部，游戏结束
}

void GameModel::reset()
{
    const float width = screenWidth;
    const float height = screenHeight;
    *this = GameModel(width, height);
}

void GameModel::resolveLeafInteractions(float dt)
{
    bool attachedToLeaf = false;
    bool standingOnGoldenLeaf = false;

    for (Leaf& leaf : leafManager.getMutableLeaves()) //遍历所有叶子
    {
        //着陆优先于抓取
        if (collisionDetector.isLanding(player,leaf))
        {
            player.landOn(leaf.getY());
            player.moveHorizontal(-LeafScrollSpeed * dt);
            const LeafEffect effect = leaf.onPlayerLanded();
            applyLeafEffect(effect);

            if (leaf.getType() == LeafType::Golden && goldenCarryDistanceRemaining > 0.0f)
            {
                standingOnGoldenLeaf = true;
                updateGoldenCarry(leaf,dt);
            }
            break;
        }

        if (player.isGrabRequested() && collisionDetector.canGrab(player,leaf))
        {
            player.moveHorizontal(-LeafScrollSpeed * dt);
            attachedToLeaf = true;
            break;
        }
    }

    if (!standingOnGoldenLeaf)
        goldenCarryDistanceRemaining = 0.0f;

    player.setHanging(attachedToLeaf);
}

void GameModel::applyLeafEffect(LeafEffect effect)
{
    switch (effect)
    {
    case LeafEffect::DarkBreakStarted:
        events.push_back({GameEventType::DarkLeafTriggered,0});
        break;

    case LeafEffect::GoldenBoostActivated:
        player.activateGoldenBoost(GoldenEffectDuration);
        goldenCarryDistanceRemaining = GoldenCarryDistance;
        events.push_back({GameEventType::GoldenBoostStarted,0});
        break;

    case LeafEffect::None:
        break;
    }
}

void GameModel::updateGoldenCarry(Leaf& leaf,float dt)
{
    player.stopMoving();
    const float carryDelta = std::min(GoldenCarrySpeed * dt,goldenCarryDistanceRemaining);
    leaf.moveHorizontal(carryDelta);
    player.moveHorizontal(carryDelta);
    goldenCarryDistanceRemaining -= carryDelta;

    if (goldenCarryDistanceRemaining <= 0.0f)
        goldenCarryDistanceRemaining = 0.0f;
}

void GameModel::updateScore(float dt)
{
    const int previousScore = getScore();
    survivalTime += dt;
    survivalScore = static_cast<int>(survivalTime); //每生存1秒加1分

    if (goldenCarryDistanceRemaining > 0.0f)
    {
        goldenBonusTime += dt;
        goldenBonusScore = static_cast<int>(goldenBonusTime * 10.0f); //携带期间每秒额外加10分
    }

    if (getScore() != previousScore)
        events.push_back({GameEventType::ScoreChanged,getScore()});
}

void GameModel::endGame()
{
    if (gameOver)
        return;
    gameOver = true;
    events.push_back({GameEventType::GameOver,getScore()});
}

void GameModel::jump() noexcept
{
    if (gameOver)
        return;

    goldenCarryDistanceRemaining = 0.0f;
    player.jump();
}

void GameModel::moveLeft() noexcept
{
    if (gameOver || goldenCarryDistanceRemaining > 0.0f)
    {
        player.stopMoving();
        return;
    }

    player.moveLeft();
}

void GameModel::moveRight() noexcept
{
    if (gameOver || goldenCarryDistanceRemaining > 0.0f)
    {
        player.stopMoving();
        return;
    }

    player.moveRight();
}

void GameModel::stopMoving() noexcept
{
    player.stopMoving();
}

void GameModel::setGrabRequested(bool requested) noexcept
{
    // 游戏结束后仍允许释放已有的抓取状态。
    if (!gameOver || !requested)
        player.setGrabRequested(requested);
}

const Player& GameModel::getPlayer() const noexcept
{
    return player;
}

const std::vector<Leaf>& GameModel::getLeaves() const noexcept
{
    return leafManager.getLeaves();
}

int GameModel::getScore() const noexcept
{
    return survivalScore + goldenBonusScore;
}

bool GameModel::isGameOver() const noexcept
{
    return gameOver;
}

std::vector<GameEvent> GameModel::takeEvents()
{
    std::vector<GameEvent> pendingEvents;
    pendingEvents.swap(events);
    return pendingEvents;
}
