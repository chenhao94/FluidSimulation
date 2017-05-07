#include "grid.hpp"
#include "particle.hpp"
#include "cmath"


void Particle::updatePos(float dt, Grid &g)
{
    const float &cs = g.getCellSize();
    int i = floor(x / cs), j = floor(y / cs);
    auto &cell = g.getGridCell(i, j);
    auto sx = (x - i * cs) / cs;
    auto sy = (y - j * cs) / cs;
    // update
    x += (g.getGridCell(i - 1, j).ux * (1 - sx) + cell.ux * sx) * dt;
    y += (g.getGridCell(i, j - 1).uy * (1 - sy) + cell.uy * sy) * dt;
    // check bound
    if (x < cs)
        x = 2 * cs - x;
    else if (x > Grid::xbound)
        x = 2 * Grid::xbound - x;
    if (y < cs)
        y = 2 * cs - y;
    else if (y > Grid::ybound)
        y = 2 * Grid::ybound - y;
    // mark grid cell as fluid
    g.markFluid(floor(x / cs), floor(y / cs));
}
