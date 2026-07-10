#pragma once

#include "Leaf.h"
#include "Player.h"

class CollisionDetector
{
public:
    bool isLanding(const Player& player,const Leaf& leaf) const noexcept;

    bool canGrab(const Player& player,const Leaf& leaf) const noexcept;
};