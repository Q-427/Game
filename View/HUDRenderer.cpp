#include "HUDRenderer.h"

#include <array>
#include <filesystem>
#include <string>

#include "../ViewModel/GameViewModel.h"

HUDRenderer::HUDRenderer()
    : m_isFontLoaded(false)
    , m_scoreFontSize(24)
    , m_titleFontSize(36)
    , m_hintFontSize(20)
{
}

bool HUDRenderer::initialize()
{
    const std::array<std::filesystem::path, 4> fontCandidates = {
        std::filesystem::path("C:/Windows/Fonts/segoeui.ttf"),
        std::filesystem::path("C:/Windows/Fonts/arial.ttf"),
        std::filesystem::path("C:/Windows/Fonts/msyh.ttc"),
        std::filesystem::path("assets/fonts/arial.ttf")};

    for (const auto& fontPath : fontCandidates)
    {
        if (std::filesystem::exists(fontPath) && m_font.openFromFile(fontPath))
        {
            m_isFontLoaded = true;
            break;
        }
    }

    return true;
}

void HUDRenderer::render(sf::RenderWindow& window, const GameViewModel& viewModel)
{
    drawScore(window, viewModel);
    drawHint(window);

    if (viewModel.isGameOver())
    {
        drawGameOver(window, viewModel);
    }
}

void HUDRenderer::drawScore(sf::RenderWindow& window, const GameViewModel& viewModel)
{
    sf::RectangleShape scorePanel(sf::Vector2f(168.0f, 44.0f));
    scorePanel.setPosition(sf::Vector2f(16.0f, 16.0f));
    scorePanel.setFillColor(sf::Color(255, 250, 236, 190));
    scorePanel.setOutlineThickness(2.0f);
    scorePanel.setOutlineColor(sf::Color(111, 140, 101, 180));
    window.draw(scorePanel);

    if (!m_isFontLoaded)
    {
        return;
    }

    sf::Text scoreText(m_font);
    scoreText.setCharacterSize(m_scoreFontSize);
    scoreText.setFillColor(sf::Color(55, 72, 50));
    scoreText.setString("Score: " + std::to_string(viewModel.getScore()));
    scoreText.setPosition(sf::Vector2f(28.0f, 22.0f));
    window.draw(scoreText);
}

void HUDRenderer::drawGameOver(sf::RenderWindow& window, const GameViewModel& viewModel)
{
    if (!viewModel.isGameOver())
    {
        return;
    }

    sf::RectangleShape overlay;
    overlay.setSize(
        sf::Vector2f(
            static_cast<float>(window.getSize().x),
            static_cast<float>(window.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(overlay);

    sf::RectangleShape panel(sf::Vector2f(360.0f, 180.0f));
    panel.setOrigin(sf::Vector2f(180.0f, 90.0f));
    panel.setPosition(
        sf::Vector2f(
            static_cast<float>(window.getSize().x) / 2.0f,
            static_cast<float>(window.getSize().y) / 2.0f));
    panel.setFillColor(sf::Color(31, 43, 41, 210));
    panel.setOutlineThickness(3.0f);
    panel.setOutlineColor(sf::Color(239, 224, 171, 220));
    window.draw(panel);

    if (!m_isFontLoaded)
    {
        return;
    }

    sf::Text gameOverText(m_font);
    gameOverText.setCharacterSize(m_titleFontSize);
    gameOverText.setFillColor(sf::Color(255, 246, 220));
    gameOverText.setString("Game Over");
    gameOverText.setPosition(
        sf::Vector2f(
            static_cast<float>(window.getSize().x) / 2.0f - 112.0f,
            static_cast<float>(window.getSize().y) / 2.0f - 48.0f));

    sf::Text restartHintText(m_font);
    restartHintText.setCharacterSize(m_hintFontSize);
    restartHintText.setFillColor(sf::Color(230, 238, 218));
    restartHintText.setString("Press R to restart");
    restartHintText.setPosition(
        sf::Vector2f(
            static_cast<float>(window.getSize().x) / 2.0f - 104.0f,
            static_cast<float>(window.getSize().y) / 2.0f + 22.0f));

    window.draw(gameOverText);
    window.draw(restartHintText);
}

void HUDRenderer::drawHint(sf::RenderWindow& window) const
{
    sf::RectangleShape hintPanel(sf::Vector2f(380.0f, 42.0f));
    hintPanel.setPosition(sf::Vector2f(16.0f, 68.0f));
    hintPanel.setFillColor(sf::Color(255, 250, 236, 155));
    hintPanel.setOutlineThickness(1.5f);
    hintPanel.setOutlineColor(sf::Color(132, 157, 118, 150));
    window.draw(hintPanel);

    if (!m_isFontLoaded)
    {
        return;
    }

    sf::Text hintText(m_font);
    hintText.setCharacterSize(18);
    hintText.setFillColor(sf::Color(70, 81, 65));
    hintText.setString("Space: Jump   Left/Right: Move   Z: Grab");
    hintText.setPosition(sf::Vector2f(28.0f, 78.0f));
    window.draw(hintText);
}
