#pragma once

#include <SFML/Window/Event.hpp>

class ICommand;

class InputHandler
{
public:
    InputHandler(
        ICommand& jumpCommand,
        ICommand& moveLeftCommand,
        ICommand& moveRightCommand,
        ICommand& stopHorizontalCommand,
        ICommand& startGrabCommand,
        ICommand& stopGrabCommand,
        ICommand& restartCommand);

    bool handleEvent(const sf::Event& event);

private:
    ICommand& m_jumpCommand;
    ICommand& m_moveLeftCommand;
    ICommand& m_moveRightCommand;
    ICommand& m_stopHorizontalCommand;
    ICommand& m_startGrabCommand;
    ICommand& m_stopGrabCommand;
    ICommand& m_restartCommand;
};
