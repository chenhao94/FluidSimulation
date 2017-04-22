#include "grid.hpp"
#include "particle.hpp"
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;

static constexpr auto g = 9.8;
static constexpr auto ra = 1.225; // density of air

Grid::Grid(int _d, int _w, float cell, float _r, int _p) : 
    d(_d), w(_w), particleNum(_p), r(_r), cellsize(cell)
{
    g = new GridCell[d * w];
    p.reserve(particleNum * particleNum * 20);
    Particle::xbound = cellsize * (d - 1);
    Particle::ybound = cellsize * (w - 1);
}

void Grid::reset()
{
    t = 0;
    memset(g, 0, d * w * sizeof(GridCell));
    p.clear();
}

void Grid::addFluid(int x, int y)
{
    g[getIndex(x, y)].fluid = true;
    // add marker particles
    for (float i = 0.5; i < particleNum; i += 1.)
        for (float j = 0.5; j < particleNum; j += 1.)
            p.emplace_back(Particle((x + i / particleNum) * cellsize, 
                                    (y + j / particleNum) * cellsize));
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
    for (int i = 0; i < d - 1; ++i)
        for (int j = 0; j < w - 1; ++j)
        {
            int pos = getIndex(i, j);
            g[pos].vx = g[pos].ux;
            g[pos].ux -= g[pos].px / r * dt;
            g[pos].vy = g[pos].uy;
            g[pos].uy -= g[pos].py / r * dt;
        }

    // update particles
    for (auto & i : p)
        i.updatePos(dt, *this);

    // update pressure
    for (int i = 0; i < d - 1; ++i)
        for (int j = 0; j < w - 1; ++j)
        {
            int pos = getIndex(i, j);
        }


    t += dt;
    return t;
}
