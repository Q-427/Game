#include "InputHandler.h"

#include <SFML/Window/Keyboard.hpp>

#include "../Common/ICommand.h"

InputHandler::InputHandler(
    ICommand& jumpCommand,
    ICommand& moveLeftCommand,
    ICommand& moveRightCommand,
    ICommand& stopHorizontalCommand,
    ICommand& startGrabCommand,
    ICommand& stopGrabCommand,
    ICommand& restartCommand)
    : m_jumpCommand(jumpCommand)
    , m_moveLeftCommand(moveLeftCommand)
    , m_moveRightCommand(moveRightCommand)
    , m_stopHorizontalCommand(stopHorizontalCommand)
    , m_startGrabCommand(startGrabCommand)
    , m_stopGrabCommand(stopGrabCommand)
    , m_restartCommand(restartCommand)
{
}

bool InputHandler::handleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::Closed>())
    {
        return false;
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        switch (keyPressed->code)
        {
        case sf::Keyboard::Key::Space:
            m_jumpCommand.execute();
            break;
        case sf::Keyboard::Key::Left:
        case sf::Keyboard::Key::A:
            m_moveLeftCommand.execute();
            break;
        case sf::Keyboard::Key::Right:
        case sf::Keyboard::Key::D:
            m_moveRightCommand.execute();
            break;
        case sf::Keyboard::Key::Z:
        case sf::Keyboard::Key::X:
            m_startGrabCommand.execute();
            break;
        case sf::Keyboard::Key::R:
            m_restartCommand.execute();
            break;
        default:
            break;
        }
    }

    if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        switch (keyReleased->code)
        {
        case sf::Keyboard::Key::Left:
        case sf::Keyboard::Key::A:
        case sf::Keyboard::Key::Right:
        case sf::Keyboard::Key::D:
            m_stopHorizontalCommand.execute();
            break;
        case sf::Keyboard::Key::Z:
        case sf::Keyboard::Key::X:
            m_stopGrabCommand.execute();
            break;
        default:
            break;
        }
    }

    return true;
}
