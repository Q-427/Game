#pragma once

#include "../View/GameWindow.h"
#include "../ViewModel/GameViewModel.h"

class GameApplication
{
public:
    GameApplication();
    ~GameApplication();
    int run();

private:
    void bindView();
    void bindViewModel();

private:
    static constexpr unsigned int WindowWidth = 1280;
    static constexpr unsigned int WindowHeight = 720;

    GameViewModel m_viewModel;
    GameWindow m_gameWindow;
    INotifyPropertyChanged::SubscriptionId m_viewModelSubscriptionId;
};
