#include "grid.hpp"
#include "particle.hpp"
#include <cmath>
#include <cstring>
#include <algorithm>
#include <Eigen/IterativeLinearSolvers>

using namespace std;

constexpr auto g = 9.8;
vector<int> Grid::fluidCells;


Grid::Grid(int _d, int _w, float cell, float _r) : d(_d), w(_w), r(_r), cellsize(cell)
{
    g.resize(d * w);
    Particle::xbound = cellsize * d;
    Particle::ybound = cellsize * w;
}

void Grid::reset()
{
    t = 0;
    g.fill(g.begin(), g.end(), GridCell());
}

void Grid::addFluid(int x, int y)
{
    markFluid(x, y);
    // TODO: add marker particles
}

inline void Grid::markFluid(int x, int y)
{
    int pos = getIndex(x, y);
    fluidCells.push_back(pos);
    g[pos].tid = fluidCells.size() - 1;
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
    // update particles and fluid grids
    for (auto &c : g)
        c.tid = -1;
    for (auto &)
    // update pressure

    t += dt;
    return t;
}
