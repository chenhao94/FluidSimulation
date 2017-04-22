#include "particle.hpp"
#include "cmath"

float Particle::xbound = 0;
float Particle::ybound = 0;

void Particle::updatePos(float dt, Grid &g)
{
    const float &cs = g.getCellSize();
    int i = floor(x / cs), j = floor(y / cs);
    auto &cell = g.getGridCell(i, j);
    // update
    x += (g.getGridCell(i-1, j).ux + cell.ux) / 2. * dt;
    y += (g.getGridCell(i, j-1).ux + cell.uy) / 2. * dt;
    // check bound
    if (x < cs)
        x = 2 * cs - x;
    else if (x > xbound)
        x = 2 * xbound - x;
    if (y < cs)
        y = 2 * cs - y;
    else if (y > ybound)
        y = 2 * ybound - y;
    // mark grid cell as fluid
    g.markFluid(floor(x / cs), floor(y / cs));
}
