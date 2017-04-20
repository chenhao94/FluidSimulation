#include "grid.hpp"
#include "particle.hpp"
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;

constexpr auto g = 9.8;

Grid::Grid(int _d, int _w, float cell, float _r) : d(_d), w(_w), r(_r), cellsize(cell)
{
    g = new GridCell[d * w];
    Particle::xbound = cellsize * d;
    Particle::ybound = cellsize * w;
}

void Grid::reset()
{
    t = 0;
    memset(g, 0, d * w * sizeof(GridCell));
}

void Grid::addFluid(int x, int y)
{
    g[getIndex(x, y)].fluid = true;
    // TODO: add marker particles
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
