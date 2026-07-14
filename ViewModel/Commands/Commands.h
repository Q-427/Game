#pragma once

#include "../../Common/ICommand.h"

class GameViewModel;

class JumpCommand : public ICommand
{
public:
    explicit JumpCommand(GameViewModel* vm) noexcept;
    void execute() override;

private:
    GameViewModel* m_vm;
};

class MoveLeftCommand : public ICommand
{
public:
    explicit MoveLeftCommand(GameViewModel* vm) noexcept;
    void execute() override;

private:
    GameViewModel* m_vm;
};

class MoveRightCommand : public ICommand
{
public:
    explicit MoveRightCommand(GameViewModel* vm) noexcept;
    void execute() override;

private:
    GameViewModel* m_vm;
};

class StopHorizontalCommand : public ICommand
{
public:
    explicit StopHorizontalCommand(GameViewModel* vm) noexcept;
    void execute() override;

private:
    GameViewModel* m_vm;
};

class StartGrabCommand : public ICommand
{
public:
    explicit StartGrabCommand(GameViewModel* vm) noexcept;
    void execute() override;

private:
    GameViewModel* m_vm;
};

class StopGrabCommand : public ICommand
{
public:
    explicit StopGrabCommand(GameViewModel* vm) noexcept;
    void execute() override;

private:
    GameViewModel* m_vm;
};

class RestartCommand : public ICommand
{
public:
    explicit RestartCommand(GameViewModel* vm) noexcept;
    void execute() override;

private:
    GameViewModel* m_vm;
};
