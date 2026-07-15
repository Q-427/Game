#pragma once

#include <functional>
#include <filesystem>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../Common/GameRenderData.h"
#include "../Common/INotifyPropertyChanged.h"
#include "GameRenderer.h"
#include "HUDRenderer.h"
#include "InputHandler.h"

class ICommand;

class GameWindow
{
public:
    GameWindow(unsigned int width, unsigned int height, const char* title);

    bool initialize();
    bool isOpen() const;
    bool processEvents();
    void render(float deltaTime);
    void playGameplayMusic();
    void playGameOverMusic();

    void setRenderData(const GameRenderData* renderData) noexcept;

    void setJumpCommand(ICommand* command) noexcept;
    void setMoveLeftCommand(ICommand* command) noexcept;
    void setMoveRightCommand(ICommand* command) noexcept;
    void setStopHorizontalCommand(ICommand* command) noexcept;
    void setStartGrabCommand(ICommand* command) noexcept;
    void setStopGrabCommand(ICommand* command) noexcept;
    void setRestartCommand(ICommand* command) noexcept;

    INotifyPropertyChanged::Handler getNotificationHandler();

private:
    void initializeAudio();
    bool tryLoadMusic(sf::Music& music, const std::filesystem::path& path, bool shouldLoop);
    void onPropertyChanged(const std::string& propertyName);

private:
    unsigned int m_width;
    unsigned int m_height;
    const char* m_title;

    sf::RenderWindow m_window;
    GameRenderer m_gameRenderer;
    HUDRenderer m_hudRenderer;
    InputHandler m_inputHandler;
    const GameRenderData* m_renderData{nullptr};
    bool m_hasPendingRenderData{true};
    sf::Music m_gameplayMusic;
    sf::Music m_gameOverMusic;
    bool m_hasGameplayMusic{false};
    bool m_hasGameOverMusic{false};
};
