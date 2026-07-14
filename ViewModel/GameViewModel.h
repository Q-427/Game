#pragma once

#include "../Common/GameRenderData.h"
#include "../Common/INotifyPropertyChanged.h"
#include "../Model/GameModel.h"
#include "Commands/Commands.h"

class GameViewModel : public INotifyPropertyChanged
{
public:
    GameViewModel(float screenWidth = 800.0f, float screenHeight = 600.0f);

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

private:
    GameModel _model;
    GameRenderData _renderData;
    JumpCommand m_jumpCommand;
    MoveLeftCommand m_moveLeftCommand;
    MoveRightCommand m_moveRightCommand;
    StopHorizontalCommand m_stopHorizontalCommand;
    StartGrabCommand m_startGrabCommand;
    StopGrabCommand m_stopGrabCommand;
    RestartCommand m_restartCommand;
};
