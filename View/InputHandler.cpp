#include "InputHandler.h"

#include <cassert>

#include <SFML/Window/Keyboard.hpp>

#include "../Common/ICommand.h"

void InputHandler::setJumpCommand(ICommand* command) noexcept
{
    m_jumpCommand = command;
}

void InputHandler::setMoveLeftCommand(ICommand* command) noexcept
{
    m_moveLeftCommand = command;
}

void InputHandler::setMoveRightCommand(ICommand* command) noexcept
{
    m_moveRightCommand = command;
}

void InputHandler::setStopHorizontalCommand(ICommand* command) noexcept
{
    m_stopHorizontalCommand = command;
}

void InputHandler::setStartGrabCommand(ICommand* command) noexcept
{
    m_startGrabCommand = command;
}

void InputHandler::setStopGrabCommand(ICommand* command) noexcept
{
    m_stopGrabCommand = command;
}

void InputHandler::setRestartCommand(ICommand* command) noexcept
{
    m_restartCommand = command;
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
            assert(m_jumpCommand != nullptr);
            m_jumpCommand->execute();
            break;
        case sf::Keyboard::Key::Left:
        case sf::Keyboard::Key::A:
            assert(m_moveLeftCommand != nullptr);
            m_moveLeftCommand->execute();
            break;
        case sf::Keyboard::Key::Right:
        case sf::Keyboard::Key::D:
            assert(m_moveRightCommand != nullptr);
            m_moveRightCommand->execute();
            break;
        case sf::Keyboard::Key::Z:
        case sf::Keyboard::Key::X:
            assert(m_startGrabCommand != nullptr);
            m_startGrabCommand->execute();
            break;
        case sf::Keyboard::Key::R:
            assert(m_restartCommand != nullptr);
            m_restartCommand->execute();
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
            assert(m_stopHorizontalCommand != nullptr);
            m_stopHorizontalCommand->execute();
            break;
        case sf::Keyboard::Key::Z:
        case sf::Keyboard::Key::X:
            assert(m_stopGrabCommand != nullptr);
            m_stopGrabCommand->execute();
            break;
        default:
            break;
        }
    }

    return true;
}
