#include "grid.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

constexpr auto g = 9.8;

void Grid::reset()
{
    t = 0;
    for (int i = 0; i < d * w; ++i)
        g[i] = GridCell(ra);
}

void Grid::addFluid(int x, int y)
{
    g[getIndex(x, y)].r = rf;
}

float Grid::step(float * buf)
{
    float vmax = 0.0;
    for (int i = 0; i < d * w; ++i)
        vmax = max(vmax, max(fabs(g[i].ux), fabs(g[i].uy))); 
    float dt = 0.1;
    float &h = cellsize;
    if (vmax > h / dt)
        dt = h / vmax;

    // update velocity
    // update pressure

    t += dt;
    return t;
}
