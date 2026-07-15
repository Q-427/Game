#include "GameViewModel.h"

GameViewModel::GameViewModel(float screenWidth, float screenHeight)
    : _model(screenWidth, screenHeight)
    , m_jumpCommand(this)
    , m_moveLeftCommand(this)
    , m_moveRightCommand(this)
    , m_stopHorizontalCommand(this)
    , m_startGrabCommand(this)
    , m_stopGrabCommand(this)
    , m_restartCommand(this)
    , m_tickCommand(this, FixedDeltaTime)
{
    rebuildRenderData();
}

const GameRenderData& GameViewModel::getRenderData() const
{
    return _renderData;
}

const GameRenderData* GameViewModel::getRenderDataPtr() const noexcept
{
    return &_renderData;
}

bool GameViewModel::isGameOver() const
{
    return _model.isGameOver();
}

ICommand* GameViewModel::getJumpCommand() noexcept
{
    return &m_jumpCommand;
}

ICommand* GameViewModel::getMoveLeftCommand() noexcept
{
    return &m_moveLeftCommand;
}

ICommand* GameViewModel::getMoveRightCommand() noexcept
{
    return &m_moveRightCommand;
}

ICommand* GameViewModel::getStopHorizontalCommand() noexcept
{
    return &m_stopHorizontalCommand;
}

ICommand* GameViewModel::getStartGrabCommand() noexcept
{
    return &m_startGrabCommand;
}

ICommand* GameViewModel::getStopGrabCommand() noexcept
{
    return &m_stopGrabCommand;
}

ICommand* GameViewModel::getRestartCommand() noexcept
{
    return &m_restartCommand;
}

ICommand* GameViewModel::getTickCommand() noexcept
{
    return &m_tickCommand;
}

void GameViewModel::update(float dt)
{
    if (_model.isGameOver())
    {
        return;
    }

    _model.update(dt);

    // Clear transient model events after the model update completes.
    _model.takeEvents();

    rebuildRenderData();
    notifyPropertyChanged("RenderData");
}

void GameViewModel::jump()
{
    _model.jump();
}

void GameViewModel::moveLeft()
{
    _model.moveLeft();
}

void GameViewModel::moveRight()
{
    _model.moveRight();
}

void GameViewModel::stopHorizontal()
{
    _model.stopMoving();
}

void GameViewModel::startGrab()
{
    _model.setGrabRequested(true);
}

void GameViewModel::stopGrab()
{
    _model.setGrabRequested(false);
}

void GameViewModel::restart()
{
    _model.reset();
    _model.takeEvents();
    rebuildRenderData();
    notifyPropertyChanged("RenderData");
}

void GameViewModel::rebuildRenderData()
{
    const auto& player = _model.getPlayer();

    _renderData.player.centerX = player.getX() + Player::CollisionWidth * 0.5f;
    _renderData.player.centerY = player.getY() + Player::CollisionHeight * 0.5f;
    _renderData.player.climbing = player.isHanging();

    _renderData.score = _model.getScore();
    _renderData.gameOver = _model.isGameOver();

    _renderData.leaves.clear();
    _renderData.leaves.reserve(_model.getLeaves().size());

    for (const auto& leaf : _model.getLeaves())
    {
        LeafRenderData viewData{};

        viewData.type = leaf.getType();
        viewData.x = leaf.getX();
        viewData.y = leaf.getY();
        viewData.width = Leaf::CollisionWidth;
        viewData.height = Leaf::CollisionHeight;
        viewData.breaking = leaf.isBreaking();

        _renderData.leaves.push_back(viewData);
    }
}
