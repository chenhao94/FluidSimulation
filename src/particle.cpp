#include "grid.hpp"
#include "particle.hpp"
#include "cmath"

float Particle::xbound = 0;
float Particle::ybound = 0;

void Particle::updatePos(float dt, Grid &g)
{
    const float &cs = g.getCellSize();
    int i = floor(x / cs), j = floor(y / cs);
    auto &cell = g.getGridCell(i, j);
    auto rx = (x - i * cs) / cs;
    auto ry = (y - j * cs) / cs;
    // update
    x += (g.getGridCell(i + 1, j).ux * (1 - rx) + cell.ux * rx) * dt;
    y += (g.getGridCell(i, j + 1).ux * (1 - ry) + cell.uy * ry) * dt;
    // check bound
    if (x < 0)
        x = - x;
    else if (x > xbound)
        x = 2 * xbound - x;
    if (y < 0)
        y = - y;
    else if (y > ybound)
        y = 2 * ybound - y;
    // mark grid cell as fluid
    g.markFluid(floor(x / cs), floor(y / cs));
}
