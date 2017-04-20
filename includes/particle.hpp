#pragma once
#include "grid.hpp"

class Particle
{
    float x, y;

    public:

    int getGridPos();

    void updatePos(GridCell cell, float dt);
};
