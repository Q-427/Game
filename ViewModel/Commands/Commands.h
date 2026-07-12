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

// 【新增】停止水平移动命令 (用于方向键弹起)
class StopHorizontalCommand : public ICommand
{
private:
  GameViewModel *_vm;

public:
  StopHorizontalCommand(GameViewModel *vm) : _vm(vm) {}
  void execute() override
  {
    if (_vm && !_vm->isGameOver())
      _vm->stopHorizontal();
  }
};

// 【新增】开始抓取命令 (用于 Z 键按下)
class StartGrabCommand : public ICommand
{
private:
  GameViewModel *_vm;

public:
  StartGrabCommand(GameViewModel *vm) : _vm(vm) {}
  void execute() override
  {
    if (_vm && !_vm->isGameOver())
      _vm->startGrab();
  }
};

// 【新增】停止抓取命令 (用于 Z 键弹起)
class StopGrabCommand : public ICommand
{
private:
  GameViewModel *_vm;

public:
  StopGrabCommand(GameViewModel *vm) : _vm(vm) {}
  void execute() override
  {
    if (_vm && !_vm->isGameOver())
      _vm->stopGrab();
  }
};

// 【新增】重新开始命令 (用于 R 键)
class RestartCommand : public ICommand
{
private:
  GameViewModel *_vm;

public:
  RestartCommand(GameViewModel *vm) : _vm(vm) {}
  void execute() override
  {
    if (_vm)
      _vm->restart();
  }
};