#pragma once

#include <SFML/Graphics.hpp>

#include "../View/GameWindow.h"
#include "../ViewModel/GameViewModel.h"

class GameApplication
{
public:
    GameApplication();
    ~GameApplication();
    int run();

private:
    void update(float deltaTime);
    bool shouldUpdateGame() const;
    void bindView();
    void bindViewModel();
    void syncMusicState();

private:
    static constexpr unsigned int WindowWidth = 1280;
    static constexpr unsigned int WindowHeight = 720;

    GameViewModel m_viewModel;
    sf::Clock m_frameClock;
    GameWindow m_gameWindow;
    INotifyPropertyChanged::SubscriptionId m_viewModelSubscriptionId;
    bool m_lastGameOverState{false};
    bool m_hasSyncedMusicState{false};
};
