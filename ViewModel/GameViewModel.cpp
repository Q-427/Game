#include "GameViewModel.h"

GameViewModel::GameViewModel()
    : _score(0), _isGameOver(false), _distanceTraveled(0.0f),
      _goldenAssistFrames(0)
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
  if (_goldenAssistFrames > 0)
  {
    _player.vx = std::max(_player.vx, 260.0f);
    _player.vy = std::min(_player.vy, -80.0f);
    --_goldenAssistFrames;
  }

  _player.applyPhysics(dt);
  _leafManager.update(dt);

  // 2. 碰撞检测与逻辑结算
  auto &leaves = _leafManager.getMutableLeaves();
  bool onGround = false;
  bool attachedToLeaf = false;

  for (auto &leaf : leaves)
  {
    // 脚底接触判定：如果踩在叶子上，可以恢复跳跃状态
    if (_collisionDetector.isOnGround(_player, leaf))
    {
      onGround = true;
      _player.landOn(leaf.y);

      if (!leaf.effectTriggered)
      {
        if (leaf.isDark())
        {
          leaf.startBreakCountdown(0.45f);
        }
        else if (leaf.isGolden())
        {
          _goldenAssistFrames = 33;
          leaf.effectTriggered = true;
          notifyPropertyChanged("GoldenAssist");
        }
      }
      break;
    }

    // 抓取判定：如果在抓取状态且产生碰撞，则免疫重力
    if (_player.isGrabbing && _collisionDetector.checkCollision(_player, leaf))
    {
      _player.vy = 0.0f;     // 强制当前 Y 方向速度归零
      attachedToLeaf = true; // 标记本帧成功抓取
      break;                 // 找到一个落脚点即可跳出循环
    }
  }
  _player.isHanging = attachedToLeaf;

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
  _player.jump();
  _player.isGrabbing = false; // 立刻解除按键抓取意图，防止下一帧瞬间重新吸附
  _player.isHanging = false;
}

void GameViewModel::moveLeft()
{
  _player.moveLeft();
}

void GameViewModel::moveRight()
{
  _player.moveRight();
}

void GameViewModel::grab()
{
  _player.grab();
}
