#include "../Common/GameRenderData.h"
#include "../Model/Leaf.h"
#include "../Model/GameModel.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace
{
int failedTests = 0;

void expect(bool condition, const std::string& message)
{
    if (!condition)
    {
        ++failedTests;
        std::cerr << "[FAIL] " << message << '\n';
    }
}

void expectNear(float actual, float expected, float tolerance, const std::string& message)
{
    expect(std::fabs(actual - expected) <= tolerance, message);
}

void testInitialState()
{
    GameModel model(1280.0f, 720.0f);

    expect(!model.isGameOver(), "new game should not be over");
    expect(model.getScore() == 0, "new game score should be zero");
    expect(model.getPlayer().isGrounded(), "player should start on the initial platform");
    expect(model.getLeaves().size() == 6, "initial leaf manager should create six leaves");
}

void testPlayerHorizontalMovement()
{
    GameModel model(1280.0f, 720.0f);

    model.jump();
    const float initialX = model.getPlayer().getX();
    model.moveRight();
    model.update(0.1f);

    expect(model.getPlayer().getX() > initialX, "moveRight should increase player x position");
    expect(model.getPlayer().getVelocityX() > 0.0f, "moveRight should set positive horizontal velocity");

    model.stopMoving();
    model.update(1.0f / 60.0f);

    expectNear(model.getPlayer().getVelocityX(), 0.0f, 0.001f, "stopMoving should clear horizontal velocity");

    const float rightX = model.getPlayer().getX();
    model.jump();
    model.moveLeft();
    model.update(0.1f);

    expect(model.getPlayer().getX() < rightX, "moveLeft should decrease player x position");
    expect(model.getPlayer().getVelocityX() < 0.0f, "moveLeft should set negative horizontal velocity");
}

void testPlayerJump()
{
    GameModel model(1280.0f, 720.0f);
    const float initialY = model.getPlayer().getY();

    model.jump();

    expect(!model.getPlayer().isGrounded(), "jump should make player leave grounded state");
    expect(model.getPlayer().getVelocityY() < 0.0f, "jump should set upward vertical velocity");

    model.update(1.0f / 60.0f);

    expect(model.getPlayer().getY() < initialY, "after jump update player should move upward");
}

void testScoreNotification()
{
    GameModel model(1280.0f, 720.0f);
    bool scoreChanged = false;

    model.subscribe(
        [&scoreChanged](const std::string& propertyName)
        {
            if (propertyName == GameModelProperty::Score)
            {
                scoreChanged = true;
            }
        });

    for (int i = 0; i < 70; ++i)
    {
        model.update(1.0f / 60.0f);
    }

    expect(model.getScore() >= 1, "score should increase after survival time passes");
    expect(scoreChanged, "score change should notify subscribers");
}

void testResetKeepsSubscription()
{
    GameModel model(1280.0f, 720.0f);
    std::vector<std::string> notifications;

    model.subscribe(
        [&notifications](const std::string& propertyName)
        {
            notifications.push_back(propertyName);
        });

    model.update(1.1f);
    model.reset();

    const bool receivedReset =
        std::find(notifications.begin(), notifications.end(), GameModelProperty::Reset) != notifications.end();

    notifications.clear();
    model.update(1.0f / 60.0f);

    expect(receivedReset, "reset should notify subscribers");
    expect(!notifications.empty(), "subscription should still work after reset");
    expect(model.getScore() == 0, "reset should clear score");
    expect(!model.isGameOver(), "reset should clear game over state");
}

void testLeafInitialStateByType()
{
    Leaf normalLeaf(LeafType::Normal, 10.0f, 20.0f);
    Leaf darkLeaf(LeafType::Dark, 30.0f, 40.0f);
    Leaf goldenLeaf(LeafType::Golden, 50.0f, 60.0f);

    expect(normalLeaf.getType() == LeafType::Normal, "normal leaf should keep its type");
    expect(darkLeaf.getType() == LeafType::Dark, "dark leaf should keep its type");
    expect(goldenLeaf.getType() == LeafType::Golden, "golden leaf should keep its type");

    expectNear(normalLeaf.getX(), 10.0f, 0.001f, "normal leaf should keep initial x");
    expectNear(darkLeaf.getY(), 40.0f, 0.001f, "dark leaf should keep initial y");

    expect(!normalLeaf.isBreaking(), "normal leaf should not start breaking");
    expect(!darkLeaf.isBreaking(), "dark leaf should not start breaking before landing");
    expect(!goldenLeaf.isBreaking(), "golden leaf should not start breaking");

    expect(!normalLeaf.isExpired(), "normal leaf should not start expired");
    expect(!darkLeaf.isExpired(), "dark leaf should not start expired");
    expect(!goldenLeaf.isExpired(), "golden leaf should not start expired");

    expect(!normalLeaf.hasTriggeredEffect(), "normal leaf should not start with triggered effect");
    expect(!darkLeaf.hasTriggeredEffect(), "dark leaf should not start with triggered effect");
    expect(!goldenLeaf.hasTriggeredEffect(), "golden leaf should not start with triggered effect");
}

void testInitialLeavesContainAllImplementedTypes()
{
    GameModel model(1280.0f, 720.0f);

    bool hasNormal = false;
    bool hasDark = false;
    bool hasGolden = false;

    for (const Leaf& leaf : model.getLeaves())
    {
        if (leaf.getType() == LeafType::Normal)
        {
            hasNormal = true;
        }
        else if (leaf.getType() == LeafType::Dark)
        {
            hasDark = true;
        }
        else if (leaf.getType() == LeafType::Golden)
        {
            hasGolden = true;
        }
    }

    expect(hasNormal, "initial leaves should contain normal leaves");
    expect(hasDark, "initial leaves should contain a dark leaf");
    expect(hasGolden, "initial leaves should contain a golden leaf");
}

void testRenderDataTypesAreCommonTypes()
{
    LeafRenderData leaf{};

    leaf.type = LeafRenderType::Dark;
    expect(leaf.type == LeafRenderType::Dark, "render leaf type should be independent common data");
}
}

int main()
{
    testInitialState();
    testPlayerHorizontalMovement();
    testPlayerJump();
    testScoreNotification();
    testResetKeepsSubscription();
    testLeafInitialStateByType();
    testInitialLeavesContainAllImplementedTypes();
    testRenderDataTypesAreCommonTypes();

    if (failedTests != 0)
    {
        std::cerr << failedTests << " test(s) failed.\n";
        return EXIT_FAILURE;
    }

    std::cout << "All model tests passed.\n";
    return EXIT_SUCCESS;
}
