#include "GameRenderer.h"

#include <array>
#include <cstdint>
#include <cmath>
#include <filesystem>

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "../ViewModel/GameViewModel.h"

GameRenderer::GameRenderer()
    : m_skyTopColor(146, 214, 255)
    , m_skyBottomColor(247, 237, 197)
    , m_groundColor(88, 132, 84)
    , m_hillBackColor(165, 204, 170)
    , m_hillFrontColor(119, 164, 114)
    , m_canopyColor(63, 109, 68)
    , m_playerBodyColor(234, 155, 128)
    , m_playerScarfColor(230, 90, 72)
    , m_playerHeadColor(255, 227, 198)
    , m_hasLayeredBackground(false)
    , m_hasPlayerTextures(false)
    , m_hasLeafTextures(false)
    , m_backgroundScroll(0.0f)
    , m_backgroundBaseSpeed(45.0f)
    , m_groundHeight(80.0f)
    , m_playerDrawWidth(60.0f)
    , m_playerDrawHeight(70.0f)
    , m_playerClimbScale(1.16f)
    , m_leafDrawSize(148.0f)
{
}

bool GameRenderer::initialize()
{
    const std::filesystem::path basePath("asserts/backgrounds/mountains_parallax");
    const std::filesystem::path playerPath("asserts/characters/cats");
    const std::filesystem::path leafPath("asserts/platforms/leaves");

    const bool hasBase = m_backgroundTexture.loadFromFile(basePath / "background.png");
    const bool hasCloudsBig = m_cloudsBigTexture.loadFromFile(basePath / "clouds_big.png");
    const bool hasCloudsSmall = m_cloudsSmallTexture.loadFromFile(basePath / "clouds_small.png");
    const bool hasMountains1 = m_mountains1Texture.loadFromFile(basePath / "mountains_1.png");
    const bool hasMountains2 = m_mountains2Texture.loadFromFile(basePath / "mountains_2.png");
    const bool hasValley = m_valleyTexture.loadFromFile(basePath / "valley.png");
    const bool hasTrees1 = m_trees1Texture.loadFromFile(basePath / "christmas_trees_1.png");
    const bool hasTrees2 = m_trees2Texture.loadFromFile(basePath / "christmas_trees_2.png");

    m_hasLayeredBackground = hasBase && hasCloudsBig && hasCloudsSmall && hasMountains1 && hasMountains2 &&
                             hasValley && hasTrees1 && hasTrees2;

    const bool hasPlayerWalk = m_playerWalkTexture.loadFromFile(playerPath / "walk.png");
    const bool hasPlayerClimb = m_playerClimbTexture.loadFromFile(playerPath / "climb.png");
    m_hasPlayerTextures = hasPlayerWalk || hasPlayerClimb;

    m_playerWalkTexture.setSmooth(false);
    m_playerClimbTexture.setSmooth(false);

    const bool hasLeafNormal = m_leafNormalTexture.loadFromFile(leafPath / "normal.png");
    const bool hasLeafDark = m_leafDarkTexture.loadFromFile(leafPath / "dark.png");
    const bool hasLeafGolden = m_leafGoldenTexture.loadFromFile(leafPath / "golden.png");
    m_hasLeafTextures = hasLeafNormal && hasLeafDark && hasLeafGolden;

    m_leafNormalTexture.setSmooth(true);
    m_leafDarkTexture.setSmooth(true);
    m_leafGoldenTexture.setSmooth(true);

    return true;
}

void GameRenderer::render(sf::RenderWindow& window, const GameViewModel& viewModel, float deltaTime)
{
    updateBackgroundScroll(deltaTime);
    drawBackground(window);
    drawLeaves(window, viewModel);
    drawPlayer(window, viewModel);
}

void GameRenderer::updateBackgroundScroll(float deltaTime)
{
    if (!m_hasLayeredBackground)
    {
        return;
    }

    m_backgroundScroll += m_backgroundBaseSpeed * deltaTime;
}

void GameRenderer::drawBackground(sf::RenderWindow& window) const
{
    if (m_hasLayeredBackground)
    {
        drawBackgroundLayer(window, m_backgroundTexture, 0.0f, 0.04f);
        drawBackgroundLayer(window, m_cloudsSmallTexture, 0.0f, 0.08f, sf::Color(255, 255, 255, 210));
        drawBackgroundLayer(window, m_cloudsBigTexture, 8.0f, 0.12f, sf::Color(255, 255, 255, 230));
        drawBackgroundLayer(window, m_mountains1Texture, 0.0f, 0.18f, sf::Color(255, 255, 255, 235));
        drawBackgroundLayer(window, m_mountains2Texture, 18.0f, 0.28f, sf::Color(255, 255, 255, 245));
        drawBackgroundLayer(window, m_valleyTexture, 28.0f, 0.42f);
        drawBackgroundLayer(window, m_trees1Texture, 32.0f, 0.58f);
        drawBackgroundLayer(window, m_trees2Texture, 40.0f, 0.76f);
        return;
    }
}

void GameRenderer::drawBackgroundLayer(
    sf::RenderWindow& window,
    const sf::Texture& texture,
    float yOffset,
    float scrollFactor,
    sf::Color tint) const
{
    const auto textureSize = texture.getSize();
    if (textureSize.x == 0 || textureSize.y == 0)
    {
        return;
    }

    sf::Sprite sprite(texture);
    sprite.setColor(tint);

    const float scaleX = static_cast<float>(window.getSize().x) / static_cast<float>(textureSize.x);
    const float scaleY = static_cast<float>(window.getSize().y) / static_cast<float>(textureSize.y);
    const float scale = std::max(scaleX, scaleY);
    sprite.setScale(sf::Vector2f(scale, scale));

    const float scaledWidth = static_cast<float>(textureSize.x) * scale;
    const float scaledHeight = static_cast<float>(textureSize.y) * scale;
    const float y = static_cast<float>(window.getSize().y) - scaledHeight + yOffset;
    const float layerScroll = std::fmod(m_backgroundScroll * scrollFactor, scaledWidth);
    const float startX = -layerScroll;

    for (int i = 0; i < 3; ++i)
    {
        sprite.setPosition(sf::Vector2f(startX + i * scaledWidth, y));
        window.draw(sprite);
    }
}

void GameRenderer::drawDistantHills(sf::RenderWindow& window) const
{
    const float width = static_cast<float>(window.getSize().x);
    const float height = static_cast<float>(window.getSize().y);

    sf::ConvexShape backHill;
    backHill.setPointCount(6);
    backHill.setPoint(0, sf::Vector2f(0.0f, height - 215.0f));
    backHill.setPoint(1, sf::Vector2f(width * 0.18f, height - 310.0f));
    backHill.setPoint(2, sf::Vector2f(width * 0.37f, height - 250.0f));
    backHill.setPoint(3, sf::Vector2f(width * 0.56f, height - 330.0f));
    backHill.setPoint(4, sf::Vector2f(width * 0.82f, height - 240.0f));
    backHill.setPoint(5, sf::Vector2f(width, height - 225.0f));
    backHill.setFillColor(m_hillBackColor);
    window.draw(backHill);

    sf::ConvexShape frontHill;
    frontHill.setPointCount(6);
    frontHill.setPoint(0, sf::Vector2f(0.0f, height - 155.0f));
    frontHill.setPoint(1, sf::Vector2f(width * 0.21f, height - 225.0f));
    frontHill.setPoint(2, sf::Vector2f(width * 0.42f, height - 185.0f));
    frontHill.setPoint(3, sf::Vector2f(width * 0.64f, height - 250.0f));
    frontHill.setPoint(4, sf::Vector2f(width * 0.86f, height - 175.0f));
    frontHill.setPoint(5, sf::Vector2f(width, height - 160.0f));
    frontHill.setFillColor(m_hillFrontColor);
    window.draw(frontHill);
}

void GameRenderer::drawCanopy(sf::RenderWindow& window) const
{
    const float width = static_cast<float>(window.getSize().x);

    sf::RectangleShape canopyBand(sf::Vector2f(width, 54.0f));
    canopyBand.setFillColor(sf::Color(49, 90, 57, 190));
    window.draw(canopyBand);

    for (int i = 0; i < 7; ++i)
    {
        sf::CircleShape crown(48.0f + (i % 2) * 10.0f);
        crown.setOrigin(sf::Vector2f(crown.getRadius(), crown.getRadius()));
        crown.setPosition(sf::Vector2f(70.0f + i * 190.0f, 38.0f + (i % 3) * 8.0f));
        crown.setFillColor(m_canopyColor);
        window.draw(crown);
    }
}

void GameRenderer::drawGround(sf::RenderWindow& window) const
{
    const float width = static_cast<float>(window.getSize().x);
    const float top = static_cast<float>(window.getSize().y) - m_groundHeight;

    sf::RectangleShape ground(sf::Vector2f(width, m_groundHeight));
    ground.setPosition(sf::Vector2f(0.0f, top));
    ground.setFillColor(m_groundColor);
    window.draw(ground);

    sf::RectangleShape mossStrip(sf::Vector2f(width, 12.0f));
    mossStrip.setPosition(sf::Vector2f(0.0f, top));
    mossStrip.setFillColor(sf::Color(124, 172, 90));
    window.draw(mossStrip);

    for (int i = 0; i < 18; ++i)
    {
        sf::RectangleShape blade(sf::Vector2f(4.0f, 18.0f + (i % 3) * 6.0f));
        blade.setOrigin(sf::Vector2f(2.0f, blade.getSize().y));
        blade.setPosition(sf::Vector2f(18.0f + i * 72.0f, top + 9.0f));
        blade.setRotation(sf::degrees(-14.0f + (i % 5) * 7.0f));
        blade.setFillColor(sf::Color(98, 149, 72));
        window.draw(blade);
    }
}

void GameRenderer::drawLeaves(sf::RenderWindow& window, const GameViewModel& viewModel) const
{
    const auto& leaves = viewModel.getLeaves();

    for (const auto& leaf : leaves)
    {
        drawLeafShape(window, leaf);
    }
}

void GameRenderer::drawLeafShape(sf::RenderWindow& window, const LeafViewData& leaf) const
{
    if (m_hasLeafTextures)
    {
        const sf::Texture* texture = &m_leafNormalTexture;
        if (leaf.type == LeafViewType::Dark)
        {
            texture = &m_leafDarkTexture;
        }
        else if (leaf.type == LeafViewType::Golden)
        {
            texture = &m_leafGoldenTexture;
        }

        const auto textureSize = texture->getSize();
        if (textureSize.x > 0 && textureSize.y > 0)
        {
            constexpr int cropLeft = 412;
            constexpr int cropTop = 408;
            constexpr int cropWidth = 1224;
            constexpr int cropHeight = 1364;

            if (leaf.type == LeafViewType::Golden)
            {
                drawGoldenGlow(window, leaf);
            }

            const float drawWidth = m_leafDrawSize;
            const float drawHeight = drawWidth * static_cast<float>(cropHeight) / static_cast<float>(cropWidth);
            const float drawX = leaf.x + leaf.width * 0.5f - drawWidth * 0.5f;
            const float drawY = leaf.y - drawHeight * 0.41f;

            sf::Sprite sprite(*texture);
            sprite.setTextureRect(sf::IntRect({cropLeft, cropTop}, {cropWidth, cropHeight}));
            sprite.setScale(
                sf::Vector2f(
                    drawWidth / static_cast<float>(cropWidth),
                    drawHeight / static_cast<float>(cropHeight)));
            sprite.setPosition(sf::Vector2f(drawX, drawY));
            sf::Color tint(188, 200, 168, 228);
            sf::Color edgeTint(156, 170, 140, 78);
            if (leaf.type == LeafViewType::Dark)
            {
                tint = sf::Color(154, 166, 146, 222);
                edgeTint = sf::Color(128, 140, 123, 74);
            }
            else if (leaf.type == LeafViewType::Golden)
            {
                tint = sf::Color(198, 186, 146, 226);
                edgeTint = sf::Color(168, 154, 112, 80);
            }

            sf::Sprite softEdge(sprite);
            softEdge.setScale(sprite.getScale() * 1.035f);
            softEdge.setPosition(sf::Vector2f(
                drawX - drawWidth * 0.0175f,
                drawY - drawHeight * 0.0175f));
            softEdge.setColor(edgeTint);
            window.draw(softEdge);

            sprite.setColor(tint);
            window.draw(sprite);
            return;
        }
    }

    const float left = leaf.x;
    const float top = leaf.y;
    const float width = leaf.width;
    const float height = leaf.height;
    const sf::Vector2f center(left + width * 0.5f, top + height * 0.5f);
    const float petalRadius = height * 0.62f;
    const float horizontalOffset = width * 0.16f;
    const float verticalOffset = height * 0.32f;

    if (leaf.type == LeafViewType::Golden)
    {
        drawGoldenGlow(window, leaf);
    }

    sf::Color fill(78, 166, 92);
    sf::Color outline(45, 110, 56);
    sf::Color vein(209, 247, 186);

    if (leaf.type == LeafViewType::Dark)
    {
        fill = sf::Color(62, 83, 79);
        outline = sf::Color(36, 52, 48);
        vein = sf::Color(160, 181, 170);
    }
    else if (leaf.type == LeafViewType::Golden)
    {
        fill = sf::Color(232, 197, 73);
        outline = sf::Color(165, 124, 38);
        vein = sf::Color(255, 244, 184);
    }

    const std::array<sf::Vector2f, 4> petalCenters = {
        sf::Vector2f(center.x - horizontalOffset, center.y - verticalOffset),
        sf::Vector2f(center.x + horizontalOffset, center.y - verticalOffset),
        sf::Vector2f(center.x - horizontalOffset, center.y + verticalOffset),
        sf::Vector2f(center.x + horizontalOffset, center.y + verticalOffset)};

    for (const auto& petalCenter : petalCenters)
    {
        sf::CircleShape petal(petalRadius, 48);
        petal.setOrigin(sf::Vector2f(petalRadius, petalRadius));
        petal.setPosition(petalCenter);
        petal.setScale(sf::Vector2f(1.1f, 0.92f));
        petal.setFillColor(fill);
        petal.setOutlineThickness(2.0f);
        petal.setOutlineColor(outline);
        window.draw(petal);
    }

    sf::CircleShape core(height * 0.32f, 40);
    core.setOrigin(sf::Vector2f(core.getRadius(), core.getRadius()));
    core.setPosition(center);
    core.setFillColor(outline);
    window.draw(core);

    sf::RectangleShape stemMain(sf::Vector2f(width * 0.09f, height * 2.4f));
    stemMain.setOrigin(sf::Vector2f(stemMain.getSize().x * 0.5f, 0.0f));
    stemMain.setPosition(sf::Vector2f(center.x, center.y + height * 0.25f));
    stemMain.setRotation(sf::degrees(8.0f));
    stemMain.setFillColor(outline);
    window.draw(stemMain);

    sf::RectangleShape stemHighlight(sf::Vector2f(width * 0.03f, height * 2.1f));
    stemHighlight.setOrigin(sf::Vector2f(stemHighlight.getSize().x * 0.5f, 0.0f));
    stemHighlight.setPosition(sf::Vector2f(center.x - width * 0.012f, center.y + height * 0.32f));
    stemHighlight.setRotation(sf::degrees(8.0f));
    stemHighlight.setFillColor(vein);
    window.draw(stemHighlight);

    drawLeafVeins(window, leaf, vein);

    if (leaf.type == LeafViewType::Dark && leaf.breaking)
    {
        std::array<sf::RectangleShape, 3> cracks = {
            sf::RectangleShape(sf::Vector2f(width * 0.12f, 2.0f)),
            sf::RectangleShape(sf::Vector2f(width * 0.11f, 2.0f)),
            sf::RectangleShape(sf::Vector2f(width * 0.1f, 2.0f))};

        cracks[0].setPosition(sf::Vector2f(center.x - width * 0.18f, center.y - height * 0.46f));
        cracks[1].setPosition(sf::Vector2f(center.x + width * 0.1f, center.y - height * 0.08f));
        cracks[2].setPosition(sf::Vector2f(center.x - width * 0.02f, center.y + height * 0.34f));
        cracks[0].setRotation(sf::degrees(-24.0f));
        cracks[1].setRotation(sf::degrees(28.0f));
        cracks[2].setRotation(sf::degrees(-48.0f));

        for (auto& crack : cracks)
        {
            crack.setFillColor(sf::Color(28, 38, 36, 210));
            window.draw(crack);
        }
    }
}

void GameRenderer::drawLeafVeins(sf::RenderWindow& window, const LeafViewData& leaf, sf::Color veinColor) const
{
    const float width = leaf.width;
    const float height = leaf.height;
    const sf::Vector2f center(leaf.x + width * 0.5f, leaf.y + height * 0.5f);

    const std::array<sf::Vector2f, 4> ends = {
        sf::Vector2f(center.x - width * 0.13f, center.y - height * 0.33f),
        sf::Vector2f(center.x + width * 0.13f, center.y - height * 0.33f),
        sf::Vector2f(center.x - width * 0.13f, center.y + height * 0.33f),
        sf::Vector2f(center.x + width * 0.13f, center.y + height * 0.33f)};

    for (const auto& end : ends)
    {
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        line[0].position = center;
        line[1].position = end;
        line[0].color = veinColor;
        line[1].color = veinColor;
        window.draw(line);
    }
}

void GameRenderer::drawGoldenGlow(sf::RenderWindow& window, const LeafViewData& leaf) const
{
    const sf::Vector2f center(leaf.x + leaf.width * 0.52f, leaf.y + leaf.height * 0.48f);

    for (int i = 0; i < 3; ++i)
    {
        sf::CircleShape glow(leaf.height * (0.78f + i * 0.34f));
        glow.setOrigin(sf::Vector2f(glow.getRadius(), glow.getRadius()));
        glow.setPosition(center);
        glow.setScale(sf::Vector2f(2.0f + i * 0.2f, 0.94f + i * 0.06f));
        glow.setFillColor(sf::Color(221, 196, 112, static_cast<std::uint8_t>(34 - i * 8)));
        window.draw(glow);
    }

    for (int i = 0; i < 4; ++i)
    {
        sf::CircleShape sparkle(2.5f + i % 2);
        sparkle.setOrigin(sf::Vector2f(sparkle.getRadius(), sparkle.getRadius()));
        sparkle.setPosition(
            sf::Vector2f(
                center.x - 36.0f + i * 22.0f,
                center.y - 16.0f + (i % 2) * 18.0f));
        sparkle.setFillColor(sf::Color(236, 224, 185, 96));
        window.draw(sparkle);
    }
}

void GameRenderer::drawPlayer(sf::RenderWindow& window, const GameViewModel& viewModel) const
{
    const sf::Vector2f center(viewModel.getPlayerCenterX(), viewModel.getPlayerCenterY());

    if (m_hasPlayerTextures)
    {
        const sf::Texture* texture = nullptr;
        bool useClimbTexture = false;
        const bool hasWalkTexture =
            m_playerWalkTexture.getSize().x > 0 && m_playerWalkTexture.getSize().y > 0;
        const bool hasClimbTexture =
            m_playerClimbTexture.getSize().x > 0 && m_playerClimbTexture.getSize().y > 0;

        if (viewModel.isPlayerClimbing() && hasClimbTexture)
        {
            texture = &m_playerClimbTexture;
            useClimbTexture = true;
        }
        else if (hasWalkTexture)
        {
            texture = &m_playerWalkTexture;
        }
        else if (hasClimbTexture)
        {
            texture = &m_playerClimbTexture;
            useClimbTexture = true;
        }

        if (texture != nullptr)
        {
            const auto textureSize = texture->getSize();
            const float drawHeight = m_playerDrawHeight * (useClimbTexture ? m_playerClimbScale : 1.0f);
            const float drawWidth =
                drawHeight * static_cast<float>(textureSize.x) / static_cast<float>(textureSize.y);

            sf::CircleShape shadow(drawWidth * 0.28f);
            shadow.setOrigin(sf::Vector2f(shadow.getRadius(), shadow.getRadius() * 0.6f));
            shadow.setScale(sf::Vector2f(1.45f, 0.42f));
            shadow.setPosition(sf::Vector2f(center.x + 1.0f, center.y + drawHeight * 0.42f));
            shadow.setFillColor(sf::Color(38, 52, 40, 58));
            window.draw(shadow);

            sf::Sprite sprite(*texture);
            sprite.setOrigin(sf::Vector2f(
                static_cast<float>(textureSize.x) * 0.5f,
                static_cast<float>(textureSize.y) * 0.5f));
            sprite.setScale(sf::Vector2f(
                drawWidth / static_cast<float>(textureSize.x),
                drawHeight / static_cast<float>(textureSize.y)));
            sprite.setPosition(center);
            window.draw(sprite);
            return;
        }
    }

    sf::CircleShape shadow(m_playerDrawWidth * 0.34f);
    shadow.setOrigin(sf::Vector2f(shadow.getRadius(), shadow.getRadius() * 0.6f));
    shadow.setScale(sf::Vector2f(1.4f, 0.45f));
    shadow.setPosition(sf::Vector2f(center.x + 2.0f, center.y + m_playerDrawHeight * 0.62f));
    shadow.setFillColor(sf::Color(40, 58, 40, 70));
    window.draw(shadow);

    sf::CircleShape head(13.0f);
    head.setOrigin(sf::Vector2f(13.0f, 13.0f));
    head.setPosition(sf::Vector2f(center.x, center.y - 18.0f));
    head.setFillColor(m_playerHeadColor);
    window.draw(head);

    sf::ConvexShape hair;
    hair.setPointCount(5);
    hair.setPoint(0, sf::Vector2f(center.x - 14.0f, center.y - 26.0f));
    hair.setPoint(1, sf::Vector2f(center.x - 5.0f, center.y - 35.0f));
    hair.setPoint(2, sf::Vector2f(center.x + 11.0f, center.y - 33.0f));
    hair.setPoint(3, sf::Vector2f(center.x + 16.0f, center.y - 18.0f));
    hair.setPoint(4, sf::Vector2f(center.x - 10.0f, center.y - 14.0f));
    hair.setFillColor(sf::Color(91, 63, 49));
    window.draw(hair);

    sf::RectangleShape body(sf::Vector2f(24.0f, 30.0f));
    body.setOrigin(sf::Vector2f(12.0f, 4.0f));
    body.setPosition(sf::Vector2f(center.x, center.y - 2.0f));
    body.setRotation(sf::degrees(-4.0f));
    body.setFillColor(m_playerBodyColor);
    window.draw(body);

    sf::RectangleShape legLeft(sf::Vector2f(7.0f, 22.0f));
    legLeft.setOrigin(sf::Vector2f(3.5f, 0.0f));
    legLeft.setPosition(sf::Vector2f(center.x - 7.0f, center.y + 16.0f));
    legLeft.setRotation(sf::degrees(8.0f));
    legLeft.setFillColor(sf::Color(86, 75, 112));
    window.draw(legLeft);

    sf::RectangleShape legRight(sf::Vector2f(7.0f, 22.0f));
    legRight.setOrigin(sf::Vector2f(3.5f, 0.0f));
    legRight.setPosition(sf::Vector2f(center.x + 6.0f, center.y + 16.0f));
    legRight.setRotation(sf::degrees(-10.0f));
    legRight.setFillColor(sf::Color(70, 64, 98));
    window.draw(legRight);

    sf::RectangleShape armLeft(sf::Vector2f(6.0f, 20.0f));
    armLeft.setOrigin(sf::Vector2f(3.0f, 2.0f));
    armLeft.setPosition(sf::Vector2f(center.x - 13.0f, center.y - 4.0f));
    armLeft.setRotation(sf::degrees(18.0f));
    armLeft.setFillColor(m_playerHeadColor);
    window.draw(armLeft);

    sf::RectangleShape armRight(sf::Vector2f(6.0f, 20.0f));
    armRight.setOrigin(sf::Vector2f(3.0f, 2.0f));
    armRight.setPosition(sf::Vector2f(center.x + 13.0f, center.y - 5.0f));
    armRight.setRotation(sf::degrees(-22.0f));
    armRight.setFillColor(m_playerHeadColor);
    window.draw(armRight);

    sf::ConvexShape scarf;
    scarf.setPointCount(4);
    scarf.setPoint(0, sf::Vector2f(center.x - 10.0f, center.y - 6.0f));
    scarf.setPoint(1, sf::Vector2f(center.x + 10.0f, center.y - 8.0f));
    scarf.setPoint(2, sf::Vector2f(center.x + 8.0f, center.y + 0.0f));
    scarf.setPoint(3, sf::Vector2f(center.x - 8.0f, center.y + 2.0f));
    scarf.setFillColor(m_playerScarfColor);
    window.draw(scarf);

    sf::RectangleShape scarfTail(sf::Vector2f(8.0f, 22.0f));
    scarfTail.setOrigin(sf::Vector2f(4.0f, 2.0f));
    scarfTail.setPosition(sf::Vector2f(center.x + 7.0f, center.y - 2.0f));
    scarfTail.setRotation(sf::degrees(28.0f));
    scarfTail.setFillColor(m_playerScarfColor);
    window.draw(scarfTail);
}
