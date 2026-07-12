#include "GameApplication.h"

#include <algorithm>

GameApplication::GameApplication()
    : m_viewModel(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight))
    , m_jumpCommand(&m_viewModel)
    , m_moveLeftCommand(&m_viewModel)
    , m_moveRightCommand(&m_viewModel)
    , m_stopHorizontalCommand(&m_viewModel)
    , m_startGrabCommand(&m_viewModel)
    , m_stopGrabCommand(&m_viewModel)
    , m_restartCommand(&m_viewModel)
    , m_window()
    , m_frameClock()
    , m_gameRenderer()
    , m_hudRenderer()
    , m_inputHandler(
          m_jumpCommand,
          m_moveLeftCommand,
          m_moveRightCommand,
          m_stopHorizontalCommand,
          m_startGrabCommand,
          m_stopGrabCommand,
          m_restartCommand)
    , m_isRunning(true)
    , m_windowTitle("Wind Leaf Runner")
{
}

int GameApplication::run()
{
    m_window.create(
        sf::VideoMode({WindowWidth, WindowHeight}),
        m_windowTitle,
        sf::Style::Titlebar | sf::Style::Close);

    m_window.setFramerateLimit(60);

    if (!m_gameRenderer.initialize())
    {
        return 1;
    }

    if (!m_hudRenderer.initialize())
    {
        return 1;
    }

    m_frameClock.restart();

    while (m_window.isOpen() && m_isRunning)
    {
        processEvents();

        const float deltaTime = std::min(m_frameClock.restart().asSeconds(), 1.0f / 30.0f);
        if (shouldUpdateGame())
        {
            update(deltaTime);
        }

        render(deltaTime);
    }

    return 0;
}

void GameApplication::processEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        const bool shouldContinue = m_inputHandler.handleEvent(*event);
        if (!shouldContinue)
        {
            m_isRunning = false;
            m_window.close();
            return;
        }
    }
}

void GameApplication::update(float deltaTime)
{
    m_viewModel.update(deltaTime);
}

void GameApplication::render(float deltaTime)
{
    m_window.clear(sf::Color(135, 206, 235));
    m_gameRenderer.render(m_window, m_viewModel, deltaTime);
    m_hudRenderer.render(m_window, m_viewModel);
    m_window.display();
}

bool GameApplication::shouldUpdateGame() const
{
    return !m_viewModel.isGameOver();
}
