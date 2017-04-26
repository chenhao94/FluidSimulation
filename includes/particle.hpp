#pragma once

class Grid;

struct Particle
{
    float x, y;

    public:

    Particle(float _x = 0.0, float _y = 0.0) : x(_x), y(_y) {}

    void updatePos(float dt, Grid & g);
};
