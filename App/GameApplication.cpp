#include "GameApplication.h"

#include <algorithm>

GameApplication::GameApplication()
    : m_viewModel(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight))
    , m_frameClock()
    , m_gameWindow(WindowWidth, WindowHeight, "Wind Leaf Runner")
    , m_viewModelSubscriptionId(0)
{
    bindView();
    bindViewModel();
}

GameApplication::~GameApplication()
{
    if (m_viewModelSubscriptionId != 0)
    {
        m_viewModel.unsubscribe(m_viewModelSubscriptionId);
    }
}

int GameApplication::run()
{
    if (!m_gameWindow.initialize())
    {
        return 1;
    }

    m_frameClock.restart();

    while (m_gameWindow.isOpen())
    {
        if (!m_gameWindow.processEvents())
        {
            break;
        }

        const float deltaTime = std::min(m_frameClock.restart().asSeconds(), 1.0f / 30.0f);
        if (shouldUpdateGame())
        {
            update(deltaTime);
        }

        m_gameWindow.render(deltaTime);
    }

    return 0;
}

void GameApplication::update(float deltaTime)
{
    m_viewModel.update(deltaTime);
}

bool GameApplication::shouldUpdateGame() const
{
    return !m_viewModel.getRenderData().gameOver;
}

void GameApplication::bindView()
{
    m_gameWindow.setRenderData(m_viewModel.getRenderDataPtr());
    m_gameWindow.setJumpCommand(m_viewModel.getJumpCommand());
    m_gameWindow.setMoveLeftCommand(m_viewModel.getMoveLeftCommand());
    m_gameWindow.setMoveRightCommand(m_viewModel.getMoveRightCommand());
    m_gameWindow.setStopHorizontalCommand(m_viewModel.getStopHorizontalCommand());
    m_gameWindow.setStartGrabCommand(m_viewModel.getStartGrabCommand());
    m_gameWindow.setStopGrabCommand(m_viewModel.getStopGrabCommand());
    m_gameWindow.setRestartCommand(m_viewModel.getRestartCommand());
}

void GameApplication::bindViewModel()
{
    m_viewModelSubscriptionId = m_viewModel.subscribe(m_gameWindow.getNotificationHandler());
}
