#include "GameViewModel.h"

GameViewModel::GameViewModel()
    : _score(0), _isGameOver(false), _distanceTraveled(0.0f)
{
}

// --- Getter 实现 ---
float GameViewModel::getPlayerX() const { return _player.x; }
float GameViewModel::getPlayerY() const { return _player.y; }
const std::vector<Leaf> &GameViewModel::getLeaves() const { return _leafManager.getLeaves(); }
int GameViewModel::getScore() const { return _score; }
bool GameViewModel::isGameOver() const { return _isGameOver; }

// --- 核心逻辑驱动 ---
void GameViewModel::update(float dt)
{
  if (_isGameOver)
    return;

  // 1. 驱动 Model 层进行物理和滚动计算
  _player.applyPhysics(dt);
  _leafManager.update(dt);

  // 2. 碰撞检测与逻辑结算
  const auto &leaves = _leafManager.getLeaves();
  bool onGround = false;

  for (const auto &leaf : leaves)
  {
    // 脚底接触判定：如果踩在叶子上，可以恢复跳跃状态
    if (_collisionDetector.isOnGround(_player, leaf))
    {
      onGround = true;
      // TODO: 如果是深色叶子，触发破碎倒计时
      // TODO: 如果是金叶子/四叶草，触发特殊效果
      break;
    }

    // 抓取判定：如果在抓取状态且产生碰撞，则悬挂减速
    if (_player.isGrabbing && _collisionDetector.checkCollision(_player, leaf))
    {
      _player.vy *= 0.5f; // 简单的悬挂减速逻辑
      break;
    }
  }

  // 3. 计分逻辑：根据玩家水平前进距离动态加分
  if (_player.vx > 0)
  {
    _distanceTraveled += _player.vx * dt;
    int newScore = static_cast<int>(_distanceTraveled / 50.0f); // 每前进50像素得1分
    if (newScore > _score)
    {
      _score = newScore;
      notifyPropertyChanged("Score"); // 触发UI更新
    }
  }

  // 4. 失败判定：如果掉出屏幕底部 (假设屏幕高度为 600)
  if (_player.y > 600.0f)
  {
    _isGameOver = true;
    notifyPropertyChanged("GameOver"); // 触发结束画面
  }

  // 5. 每帧通知渲染层更新位置
  notifyPropertyChanged("PlayerPosition");
  notifyPropertyChanged("Leaves");
}

// --- 给 Command 调用的行为 ---
void GameViewModel::jump()
{
  _player.jump(); // 内部会设置 vy = -JUMP_SPEED
}

void GameViewModel::moveLeft()
{
  _player.vx = -200.0f; // 假设移动速度，实际应调用 Model 的方法
}

void GameViewModel::moveRight()
{
  _player.vx = 200.0f;
}

void GameViewModel::grab()
{
  // 开启抓取状态，后续在 update 中通过 CollisionDetector 判定是否生效
  _player.grab();
}