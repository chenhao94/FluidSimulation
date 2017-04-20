#include "particle.hpp"
#include "cmath"

float Particle::xbound = 0;
float Particle::ybound = 0;

void Particle::updatePos(GridCell &cell, float dt, Grid &g)
{
    // update
    x += cell.ux * dt;
    y += cell.uy * dt;
    // check bound
    if (x < 0)
        x = -x;
    else if (x > xbound)
        x = 2 * xbound - x;
    if (y < 0)
        y = -y;
    else if (y > ybound)
        y = 2 * ybound - y;
    // mark grid cell as fluid
    g.markFluid(floor(x / g.getCellSize()), floor(y / g.getCellSize()));
}
