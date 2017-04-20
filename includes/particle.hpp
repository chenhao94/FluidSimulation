#pragma once
#include "grid.hpp"

class Particle
{
    float x, y;

    public:

    void updatePos(GridCell &cell, float dt, Grid & g);

    static float xbound, ybound;
};
