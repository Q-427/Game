#pragma once

#include <algorithm>

struct Vector2
{
    float x{0.0f};
    float y{0.0f};

    Vector2 operator+(const Vector2& other) const noexcept
    {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2& other) const noexcept
    {
        return {x - other.x, y - other.y};
    }

    Vector2 operator*(float scalar) const noexcept
    {
        return {x * scalar, y * scalar};
    }
};

struct Rect
{
    float x{0.0f};
    float y{0.0f};  //左上角坐标
    float width{0.0f};
    float height{0.0f};

    float right() const noexcept
    {
        return x + width;
    }

    float bottom() const noexcept
    {
        return y + height;
    }

    bool intersects(const Rect& other) const noexcept
    {
        return x < other.right() &&
               right() > other.x &&
               y < other.bottom() &&
               bottom() > other.y;
    }
};

inline float clampFloat(float value,float minimum,float maximum) noexcept
{
    return std::clamp(value, minimum, maximum);
}