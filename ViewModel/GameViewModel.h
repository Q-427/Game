#pragma once
#include "../Common/INotifyPropertyChanged.h"
#include "../Model/GameModel.h"
#include <vector>

// --- View 层专用数据结构 ---
enum class LeafViewType
{
  Normal,
  Dark,
  Golden
};

struct LeafViewData
{
  LeafViewType type;
  float x;
  float y;
  float width;
  float height;
  bool breaking;
};

class GameViewModel : public INotifyPropertyChanged
{
public:
  // 假设窗口默认大小为 800x600，也可以让 App 层传进来
  GameViewModel(float screenWidth = 800.0f, float screenHeight = 600.0f);

  // === 暴露给 View 的只读属性 ===
  float getPlayerCenterX() const;                     // 【修改】输出中心坐标
  float getPlayerCenterY() const;                     // 【修改】输出中心坐标
  const std::vector<LeafViewData> &getLeaves() const; // 【修改】返回 View 专用结构
  int getScore() const;
  bool isGameOver() const;
  bool isPlayerClimbing() const; // 玩家是否处于抓取/攀爬状态

  // === 核心驱动 ===
  void update(float dt);

  // === 暴露给 Command 的动作接口 ===
  void jump();
  void moveLeft();
  void moveRight();
  void stopHorizontal(); // 【新增】停止水平移动
  void startGrab();      // 【新增】按下抓取
  void stopGrab();       // 【新增】松开抓取
  void restart();        // 【新增】重新开始游戏

private:
  // Model 层唯一入口 (接管了所有游戏规则)
  GameModel _model;

  // 缓存的 View 数据
  std::vector<LeafViewData> _leafViews;
};