#pragma once

#include <SFML/Graphics.hpp>

class GameViewModel;
struct LeafViewData;

class GameRenderer
{
public:
    GameRenderer();
    bool initialize();
    void render(sf::RenderWindow& window, const GameViewModel& viewModel, float deltaTime);

private:
    void updateBackgroundScroll(float deltaTime);
    void drawBackground(sf::RenderWindow& window) const;
    void drawGround(sf::RenderWindow& window) const;
    void drawCanopy(sf::RenderWindow& window) const;
    void drawDistantHills(sf::RenderWindow& window) const;
    void drawBackgroundLayer(
        sf::RenderWindow& window,
        const sf::Texture& texture,
        float yOffset,
        float scrollFactor,
        sf::Color tint = sf::Color::White) const;
    void drawLeaves(sf::RenderWindow& window, const GameViewModel& viewModel) const;
    void drawLeafShape(sf::RenderWindow& window, const LeafViewData& leaf) const;
    void drawLeafVeins(sf::RenderWindow& window, const LeafViewData& leaf, sf::Color veinColor) const;
    void drawGoldenGlow(sf::RenderWindow& window, const LeafViewData& leaf) const;
    void drawPlayer(sf::RenderWindow& window, const GameViewModel& viewModel) const;

private:
    sf::Color m_skyTopColor;
    sf::Color m_skyBottomColor;
    sf::Color m_groundColor;
    sf::Color m_hillBackColor;
    sf::Color m_hillFrontColor;
    sf::Color m_canopyColor;
    sf::Color m_playerBodyColor;
    sf::Color m_playerScarfColor;
    sf::Color m_playerHeadColor;

    sf::Texture m_backgroundTexture;
    sf::Texture m_cloudsBigTexture;
    sf::Texture m_cloudsSmallTexture;
    sf::Texture m_mountains1Texture;
    sf::Texture m_mountains2Texture;
    sf::Texture m_valleyTexture;
    sf::Texture m_trees1Texture;
    sf::Texture m_trees2Texture;
    sf::Texture m_playerWalkTexture;
    sf::Texture m_playerClimbTexture;
    sf::Texture m_leafNormalTexture;
    sf::Texture m_leafDarkTexture;
    sf::Texture m_leafGoldenTexture;

    bool m_hasLayeredBackground;
    bool m_hasPlayerTextures;
    bool m_hasLeafTextures;
    float m_backgroundScroll;
    float m_backgroundBaseSpeed;

    float m_groundHeight;
    float m_playerDrawWidth;
    float m_playerDrawHeight;
    float m_playerClimbScale;
    float m_leafDrawSize;
};
