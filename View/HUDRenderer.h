#pragma once

#include <SFML/Graphics.hpp>

class GameViewModel;

class HUDRenderer
{
public:
    HUDRenderer();
    bool initialize();
    void render(sf::RenderWindow& window, const GameViewModel& viewModel);

private:
    void drawScore(sf::RenderWindow& window, const GameViewModel& viewModel);
    void drawGameOver(sf::RenderWindow& window, const GameViewModel& viewModel);
    void drawHint(sf::RenderWindow& window) const;

private:
    sf::Font m_font;
    bool m_isFontLoaded;

    unsigned int m_scoreFontSize;
    unsigned int m_titleFontSize;
    unsigned int m_hintFontSize;
};
