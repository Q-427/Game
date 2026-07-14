#include "Commands.h"

#include "../GameViewModel.h"

JumpCommand::JumpCommand(GameViewModel* vm) noexcept
    : m_vm(vm)
{
}

void JumpCommand::execute()
{
    if (m_vm && !m_vm->isGameOver())
    {
        m_vm->jump();
    }
}

MoveLeftCommand::MoveLeftCommand(GameViewModel* vm) noexcept
    : m_vm(vm)
{
}

void MoveLeftCommand::execute()
{
    if (m_vm && !m_vm->isGameOver())
    {
        m_vm->moveLeft();
    }
}

MoveRightCommand::MoveRightCommand(GameViewModel* vm) noexcept
    : m_vm(vm)
{
}

void MoveRightCommand::execute()
{
    if (m_vm && !m_vm->isGameOver())
    {
        m_vm->moveRight();
    }
}

StopHorizontalCommand::StopHorizontalCommand(GameViewModel* vm) noexcept
    : m_vm(vm)
{
}

void StopHorizontalCommand::execute()
{
    if (m_vm && !m_vm->isGameOver())
    {
        m_vm->stopHorizontal();
    }
}

StartGrabCommand::StartGrabCommand(GameViewModel* vm) noexcept
    : m_vm(vm)
{
}

void StartGrabCommand::execute()
{
    if (m_vm && !m_vm->isGameOver())
    {
        m_vm->startGrab();
    }
}

StopGrabCommand::StopGrabCommand(GameViewModel* vm) noexcept
    : m_vm(vm)
{
}

void StopGrabCommand::execute()
{
    if (m_vm && !m_vm->isGameOver())
    {
        m_vm->stopGrab();
    }
}

RestartCommand::RestartCommand(GameViewModel* vm) noexcept
    : m_vm(vm)
{
}

void RestartCommand::execute()
{
    if (m_vm)
    {
        m_vm->restart();
    }
}
