#include "GameWindow.h"

#include <cassert>

#include "../Common/ICommand.h"

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
    syncAudioState();

    if (m_renderData != nullptr)
    {
        render(0.0f);
    }

    return true;
}

bool GameWindow::isOpen() const
{
    return m_window.isOpen();
}

bool GameWindow::runFrame()
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

    m_nextRenderDeltaTime = FixedDeltaTime;

    if (m_tickCommand != nullptr)
    {
        m_tickCommand->execute();
    }

    m_nextRenderDeltaTime = 0.0f;
    return true;
}

void GameWindow::render(float deltaTime)
{
    assert(m_renderData != nullptr);

    m_window.clear(sf::Color(135, 206, 235));
    m_gameRenderer.render(m_window, *m_renderData, deltaTime);
    m_hudRenderer.render(m_window, *m_renderData);
    m_window.display();
}

void GameWindow::setRenderData(const GameRenderData* renderData) noexcept
{
    m_renderData = renderData;
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

void GameWindow::setTickCommand(ICommand* command) noexcept
{
    m_tickCommand = command;
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

void GameWindow::syncAudioState()
{
    if (m_renderData == nullptr)
    {
        return;
    }

    const bool isGameOver = m_renderData->gameOver;
    if (m_hasAudioStateInitialized && isGameOver == m_lastGameOverState)
    {
        return;
    }

    if (isGameOver)
    {
        m_gameplayMusic.stop();

        if (m_hasGameOverMusic)
        {
            m_gameOverMusic.play();
        }
    }
    else
    {
        m_gameOverMusic.stop();

        if (m_hasGameplayMusic &&
            m_gameplayMusic.getStatus() != sf::SoundSource::Status::Playing)
        {
            m_gameplayMusic.play();
        }
    }

    m_lastGameOverState = isGameOver;
    m_hasAudioStateInitialized = true;
}

void GameWindow::onPropertyChanged(const std::string& propertyName)
{
    if (propertyName == "RenderData")
    {
        syncAudioState();
        render(m_nextRenderDeltaTime);
    }
}
