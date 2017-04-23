#include "grid.hpp"
#include "particle.hpp"
#include <cmath>
#include <cstring>
#include <algorithm>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>

#include <iostream>

using namespace std;

static constexpr auto g = 9.8;
vector<int> Grid::fluidCells;

Grid::Grid(int _d, int _w, float cell, float _r, int _p) : 
    D(_d), W(_w), particleNum(_p), r(_r), cellsize(cell)
{
    gc.resize(D * W);
    Particle::xbound = cellsize * (D - 1);
    Particle::ybound = cellsize * (W - 1);
    p.reserve(particleNum * particleNum * 20);
}

void Grid::reset()
{
    t = 0;
    fill(gc.begin(), gc.end(), GridCell());
    p.clear();
}

void Grid::addFluid(int x, int y)
{
    markFluid(x, y);
    // add marker particles
    for (float i = 0.5; i < particleNum; i += 1.)
        for (float j = 0.5; j < particleNum; j += 1.)
            p.emplace_back(Particle((x + i / particleNum) * cellsize, 
                                    (y + j / particleNum) * cellsize));
}

void Grid::markFluid(int x, int y)
{
    int pos = getIndex(x, y);
    if (gc[pos].tid >= 0)
        return;
    fluidCells.push_back(pos);
    gc[pos].tid = fluidCells.size() - 1;
}

float Grid::step()
{
    float vmax = 0.0;
    for (int i = 0; i < D * W; ++i)
        vmax = max(vmax, max(fabs(gc[i].ux), fabs(gc[i].uy))); 
    float dt = 0.1;
    float &h = cellsize;
    if (vmax > h / dt)
        dt = h / vmax;

    // update velocity
    for (int i = 0; i < D; ++i)
        for (int j = 0 ; j < W; ++j)
        {
            int pos = getIndex(i, j);
            float cur_p = gc[pos].p;
            gc[pos].vx = gc[pos].ux;
            gc[pos].vy = gc[pos].uy;
            if (gc[pos].tid < 0)
            {
                int tpos;
                if (i > 0 && gc[tpos = getIndex(i - 1, j)].tid >= 0)
                    gc[pos].ux = gc[tpos].ux;
                else if (i < D - 1 && gc[tpos = getIndex(i + 1, j)].tid >= 0)
                    gc[pos].ux = gc[tpos].ux;
                else
                    gc[pos].ux = 0;
                if (j > 0 && gc[tpos = getIndex(i, j - 1)].tid >= 0)
                    gc[pos].uy = gc[tpos].uy;
                else if (j < W - 1 && gc[tpos = getIndex(i, j + 1)].tid >= 0)
                    gc[pos].uy = gc[tpos].uy;
                else
                    gc[pos].uy = 0;
                continue;
            }
            if (i < D - 1)
                gc[pos].ux += dt * ( g - (gc[getIndex(i + 1, j)].p - cur_p) / r / h);
            else
                gc[pos].ux = 0;
            if (j < W - 1)
                gc[pos].uy += -dt * (gc[getIndex(i, j + 1)].p - cur_p) / r / h;
            else
                gc[pos].uy = 0;

        }

    // update particles and fluid grids
    fluidCells.clear();
    for (auto &c : gc)
        c.tid = -1;
    for (auto & i : p)
        i.updatePos(dt, *this);

    // update pressure
    typedef Eigen::SparseMatrix<float, Eigen::RowMajor> MatType;
    MatType A(fluidCells.size(), fluidCells.size());
    typedef Eigen::VectorXf VecType;
    VecType b, x;

    for (int i = 0; i < D - 1; ++i)
        for (int j = 0; j < W - 1; ++j)
        {
            int pos = getIndex(i, j), tpos, id, tid;
            if ((id = gc[pos].tid) < 0)
                gc[pos].p = 0;
            else
            {
                int omega = 0;
                for (int k = i-1; k < i+2; k+=2)
                    if (0 <= k && k < D - 1)
                        for (int l = j -1; l < j+2; l+=2)
                            if (0 <= l && l < W - 1)
                            {
                                ++omega;
                                tpos = getIndex(k, l);
                                if ((tid = gc[tpos].tid) >= 0)
                                    A.insert(id, tid) = 1;
                            }
                A.insert(id, id) = -omega;
                b(id) = -getDivergence(pos) * r; 
            }
        }
    Eigen::BiCGSTAB<MatType> solver;
    x = solver.compute(A).solve(b);
    for (int i = 0; i < fluidCells.size(); ++i)
        gc[fluidCells[i]].p = x(i);

    t += dt;
    return t;
}

int Grid::getDivergence(int pos) const
{
    return gc[pos + W].ux + gc[pos + 1].uy - gc[pos].ux - gc[pos].uy; 
}
