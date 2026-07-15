#include "GameApplication.h"

GameApplication::GameApplication()
    : m_viewModel(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight))
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

    while (m_gameWindow.isOpen())
    {
        if (!m_gameWindow.runFrame())
        {
            break;
        }
    }

    return 0;
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
    m_gameWindow.setTickCommand(m_viewModel.getTickCommand());
}

void GameApplication::bindViewModel()
{
    m_viewModelSubscriptionId = m_viewModel.subscribe(m_gameWindow.getNotificationHandler());
}
