#include "GameWindow.h"

#include <cassert>

namespace
{
const std::filesystem::path GameplayMusicPath{"assets/audios/game_start.ogg"};

const std::filesystem::path GameOverMusicPath{"assets/audios/game_over.ogg"};

constexpr float MusicVolume = 60.0f;
}

GameWindow::GameWindow(unsigned int width, unsigned int height, const char* title)
    : m_width(width)
    , m_height(height)
    , m_title(title)
    , m_window()
    , m_gameRenderer()
    , m_hudRenderer()
    , m_inputHandler()
{
}

bool GameWindow::initialize()
{
    m_window.create(
        sf::VideoMode({m_width, m_height}),
        m_title,
        sf::Style::Titlebar | sf::Style::Close);

    m_window.setFramerateLimit(60);

    if (!m_gameRenderer.initialize())
    {
        return false;
    }

    if (!m_hudRenderer.initialize())
    {
        return false;
    }

    initializeAudio();

    return true;
}

bool GameWindow::isOpen() const
{
    return m_window.isOpen();
}

bool GameWindow::processEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        const bool shouldContinue = m_inputHandler.handleEvent(*event);
        if (!shouldContinue)
        {
            m_window.close();
            return false;
        }
    }

    return true;
}

void GameWindow::render(float deltaTime)
{
    assert(m_renderData != nullptr);

    m_window.clear(sf::Color(135, 206, 235));
    m_gameRenderer.render(m_window, *m_renderData, deltaTime);
    m_hudRenderer.render(m_window, *m_renderData);
    m_window.display();

    m_hasPendingRenderData = false;
}

void GameWindow::playGameplayMusic()
{
    m_gameOverMusic.stop();

    if (!m_hasGameplayMusic)
    {
        return;
    }

    if (m_gameplayMusic.getStatus() != sf::SoundSource::Status::Playing)
    {
        m_gameplayMusic.play();
    }
}

void GameWindow::playGameOverMusic()
{
    m_gameplayMusic.stop();

    if (!m_hasGameOverMusic)
    {
        return;
    }

    m_gameOverMusic.play();
}

void GameWindow::setRenderData(const GameRenderData* renderData) noexcept
{
    m_renderData = renderData;
    m_hasPendingRenderData = true;
}

void GameWindow::setJumpCommand(ICommand* command) noexcept
{
    m_inputHandler.setJumpCommand(command);
}

void GameWindow::setMoveLeftCommand(ICommand* command) noexcept
{
    m_inputHandler.setMoveLeftCommand(command);
}

void GameWindow::setMoveRightCommand(ICommand* command) noexcept
{
    m_inputHandler.setMoveRightCommand(command);
}

void GameWindow::setStopHorizontalCommand(ICommand* command) noexcept
{
    m_inputHandler.setStopHorizontalCommand(command);
}

void GameWindow::setStartGrabCommand(ICommand* command) noexcept
{
    m_inputHandler.setStartGrabCommand(command);
}

void GameWindow::setStopGrabCommand(ICommand* command) noexcept
{
    m_inputHandler.setStopGrabCommand(command);
}

void GameWindow::setRestartCommand(ICommand* command) noexcept
{
    m_inputHandler.setRestartCommand(command);
}

INotifyPropertyChanged::Handler GameWindow::getNotificationHandler()
{
    return [this](const std::string& propertyName)
    {
        onPropertyChanged(propertyName);
    };
}

void GameWindow::initializeAudio()
{
    m_hasGameplayMusic = tryLoadMusic(m_gameplayMusic, GameplayMusicPath, true);
    m_hasGameOverMusic = tryLoadMusic(m_gameOverMusic, GameOverMusicPath, false);
}

bool GameWindow::tryLoadMusic(sf::Music& music, const std::filesystem::path& path, bool shouldLoop)
{
    if (path.empty())
    {
        return false;
    }

    if (!music.openFromFile(path))
    {
        return false;
    }

    music.setLooping(shouldLoop);
    music.setVolume(MusicVolume);
    return true;
}

void GameWindow::onPropertyChanged(const std::string& propertyName)
{
    if (propertyName == "RenderData")
    {
        m_hasPendingRenderData = true;
    }
}
