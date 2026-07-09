#pragma once
#include "../Common/INotifyPropertyChanged.h"
#include "../Model/Player.h"
#include "../Model/LeafManager.h"
#include "../Model/CollisionDetector.h"
#include <vector>

class GameViewModel : public INotifyPropertyChanged
{
public:
  GameViewModel();

  // === 暴露给 View 的只读属性 ===
  float getPlayerX() const;
  float getPlayerY() const;
  const std::vector<Leaf> &getLeaves() const;
  int getScore() const;
  bool isGameOver() const;

  // === 核心驱动 ===
  void update(float dt);

  // === 暴露给 Command 的动作接口 ===
  void jump();
  void moveLeft();
  void moveRight();
  void grab();

private:
  // Model 层实例
  Player _player;
  LeafManager _leafManager;
  CollisionDetector _collisionDetector;

  // 游戏业务状态
  int _score;
  bool _isGameOver;
  float _distanceTraveled; // 用于根据前进距离动态计分
  int _goldenAssistFrames;
};
