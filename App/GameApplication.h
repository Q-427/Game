#pragma once

#include <SFML/Graphics.hpp>

#include "../View/GameRenderer.h"
#include "../View/HUDRenderer.h"
#include "../View/InputHandler.h"
#include "../ViewModel/Commands/Commands.h"
#include "../ViewModel/GameViewModel.h"

class GameApplication
{
public:
    GameApplication();
    int run();

private:
    void processEvents();
    void update(float deltaTime);
    void render(float deltaTime);
    bool shouldUpdateGame() const;

private:
    static constexpr unsigned int WindowWidth = 1280;
    static constexpr unsigned int WindowHeight = 720;

    GameViewModel m_viewModel;
    JumpCommand m_jumpCommand;
    MoveLeftCommand m_moveLeftCommand;
    MoveRightCommand m_moveRightCommand;
    StopHorizontalCommand m_stopHorizontalCommand;
    StartGrabCommand m_startGrabCommand;
    StopGrabCommand m_stopGrabCommand;
    RestartCommand m_restartCommand;

    sf::RenderWindow m_window;
    sf::Clock m_frameClock;

    GameRenderer m_gameRenderer;
    HUDRenderer m_hudRenderer;
    InputHandler m_inputHandler;

    bool m_isRunning;
    const char* m_windowTitle;
};
