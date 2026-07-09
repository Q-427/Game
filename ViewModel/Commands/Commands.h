#pragma once
#include "../Common/ICommand.h"
#include "../ViewModel/GameViewModel.h"

// 跳跃命令
class JumpCommand : public ICommand
{
private:
  GameViewModel *_vm;

public:
  JumpCommand(GameViewModel *vm) : _vm(vm) {}
  void execute() override
  {
    if (_vm && !_vm->isGameOver())
      _vm->jump();
  }
};

// 左移命令
class MoveLeftCommand : public ICommand
{
private:
  GameViewModel *_vm;

public:
  MoveLeftCommand(GameViewModel *vm) : _vm(vm) {}
  void execute() override
  {
    if (_vm && !_vm->isGameOver())
      _vm->moveLeft();
  }
};

// 右移命令
class MoveRightCommand : public ICommand
{
private:
  GameViewModel *_vm;

public:
  MoveRightCommand(GameViewModel *vm) : _vm(vm) {}
  void execute() override
  {
    if (_vm && !_vm->isGameOver())
      _vm->moveRight();
  }
};

// 抓取命令 (悬挂)
class GrabCommand : public ICommand
{
private:
  GameViewModel *_vm;

public:
  GrabCommand(GameViewModel *vm) : _vm(vm) {}
  void execute() override
  {
    if (_vm && !_vm->isGameOver())
      _vm->grab();
  }
};