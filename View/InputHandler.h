#pragma once

#include <SFML/Window/Event.hpp>

class ICommand;

class InputHandler
{
public:
    InputHandler() = default;

    void setJumpCommand(ICommand* command) noexcept;
    void setMoveLeftCommand(ICommand* command) noexcept;
    void setMoveRightCommand(ICommand* command) noexcept;
    void setStopHorizontalCommand(ICommand* command) noexcept;
    void setStartGrabCommand(ICommand* command) noexcept;
    void setStopGrabCommand(ICommand* command) noexcept;
    void setRestartCommand(ICommand* command) noexcept;

    bool handleEvent(const sf::Event& event);

private:
    ICommand* m_jumpCommand{nullptr};
    ICommand* m_moveLeftCommand{nullptr};
    ICommand* m_moveRightCommand{nullptr};
    ICommand* m_stopHorizontalCommand{nullptr};
    ICommand* m_startGrabCommand{nullptr};
    ICommand* m_stopGrabCommand{nullptr};
    ICommand* m_restartCommand{nullptr};
};
