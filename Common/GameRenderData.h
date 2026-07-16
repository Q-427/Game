#pragma once

#include <vector>

enum class LeafRenderType
{
    Normal,
    Dark,
    Golden
};

struct LeafRenderData
{
    LeafRenderType type{LeafRenderType::Normal};
    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
    bool breaking{false};
};

struct PlayerRenderData
{
    float centerX{0.0f};
    float centerY{0.0f};
    bool climbing{false};
};

struct GameRenderData
{
    PlayerRenderData player;
    std::vector<LeafRenderData> leaves;
    int score{0};
    bool gameOver{false};
};
