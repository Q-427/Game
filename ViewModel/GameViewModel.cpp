#include "GameViewModel.h"

GameViewModel::GameViewModel(float screenWidth, float screenHeight)
    : _model(screenWidth, screenHeight)
{
}

// --- Getter 实现 (数据转换) ---
float GameViewModel::getPlayerCenterX() const
{
  // 将 Model 的左上角坐标转换成 Renderer 使用的中心坐标
  return _model.getPlayer().getX() + Player::CollisionWidth * 0.5f;
}

float GameViewModel::getPlayerCenterY() const
{
  // 将 Model 的左上角坐标转换成 Renderer 使用的中心坐标
  return _model.getPlayer().getY() + Player::CollisionHeight * 0.5f;
}

const std::vector<LeafViewData> &GameViewModel::getLeaves() const
{
  return _leafViews;
}

int GameViewModel::getScore() const { return _model.getScore(); }
bool GameViewModel::isGameOver() const { return _model.isGameOver(); }
bool GameViewModel::isPlayerClimbing() const { return _model.getPlayer().isHanging(); }

// --- 核心逻辑驱动 ---
void GameViewModel::update(float dt)
{
  if (_model.isGameOver())
    return;

  // 1. 全权委托 GameModel 进行业务更新
  _model.update(dt);

  // 2. 将 Model 事件转换成属性通知
  auto events = _model.takeEvents();
  for (const auto &ev : events)
  {
    switch (ev.type)
    {
    case GameEventType::ScoreChanged:
      notifyPropertyChanged("Score");
      break;
    case GameEventType::GameOver:
      notifyPropertyChanged("GameOver");
      break;
    case GameEventType::GoldenBoostStarted:
      notifyPropertyChanged("GoldenAssist");
      break;
    case GameEventType::DarkLeafTriggered:
      notifyPropertyChanged("DarkLeafTriggered");
      break;
    }
  }

  // 3. 重建 LeafViewData 列表，隔离 Model 的 Leaf 结构
  _leafViews.clear();
  for (const auto &leaf : _model.getLeaves())
  {
    LeafViewData vd;

    // 【修正】根据真实的 LeafType 枚举进行映射
    switch (leaf.getType())
    {
    case LeafType::Dark:
      vd.type = LeafViewType::Dark;
      break;
    case LeafType::Golden:
      vd.type = LeafViewType::Golden;
      break;
    default:
      vd.type = LeafViewType::Normal;
      break;
    }

    vd.x = leaf.getX();
    vd.y = leaf.getY();

    // 【修正】真实代码中尺寸是静态常量，不是实例方法
    vd.width = Leaf::CollisionWidth;
    vd.height = Leaf::CollisionHeight;

    vd.breaking = leaf.isBreaking();

    _leafViews.push_back(vd);
  }

  // 4. 通知渲染层更新位置
  notifyPropertyChanged("PlayerPosition");
  notifyPropertyChanged("Leaves");
}

// --- 给 Command 调用的行为 (委托给 Model) ---
void GameViewModel::jump() { _model.jump(); }
void GameViewModel::moveLeft() { _model.moveLeft(); }
void GameViewModel::moveRight() { _model.moveRight(); }
void GameViewModel::stopHorizontal() { _model.stopMoving(); }
void GameViewModel::startGrab() { _model.setGrabRequested(true); }
void GameViewModel::stopGrab() { _model.setGrabRequested(false); }
void GameViewModel::restart() { _model.reset(); }