#pragma once

#include "../Common/GameRenderData.h"
#include "../Common/INotifyPropertyChanged.h"
#include "../Model/GameModel.h"
#include "Commands/Commands.h"

class GameViewModel : public INotifyPropertyChanged
{
public:
    GameViewModel(float screenWidth = 800.0f, float screenHeight = 600.0f);
    ~GameViewModel();

    const GameRenderData& getRenderData() const;
    const GameRenderData* getRenderDataPtr() const noexcept;
    bool isGameOver() const;
    ICommand* getJumpCommand() noexcept;
    ICommand* getMoveLeftCommand() noexcept;
    ICommand* getMoveRightCommand() noexcept;
    ICommand* getStopHorizontalCommand() noexcept;
    ICommand* getStartGrabCommand() noexcept;
    ICommand* getStopGrabCommand() noexcept;
    ICommand* getRestartCommand() noexcept;
    ICommand* getTickCommand() noexcept;

    void update(float dt);

    void jump();
    void moveLeft();
    void moveRight();
    void stopHorizontal();
    void startGrab();
    void stopGrab();
    void restart();

private:
    void rebuildRenderData();
    void onModelPropertyChanged(const std::string& propertyName);

private:
    static constexpr float FixedDeltaTime = 1.0f / 60.0f;

    GameModel _model;
    INotifyPropertyChanged::SubscriptionId modelSubscriptionId{0};
    GameRenderData _renderData;
    JumpCommand m_jumpCommand;
    MoveLeftCommand m_moveLeftCommand;
    MoveRightCommand m_moveRightCommand;
    StopHorizontalCommand m_stopHorizontalCommand;
    StartGrabCommand m_startGrabCommand;
    StopGrabCommand m_stopGrabCommand;
    RestartCommand m_restartCommand;
    TickCommand m_tickCommand;
};
