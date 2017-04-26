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
float Grid::xbound = 0;
float Grid::ybound = 0;

Grid::Grid(int _d, int _w, float cell, float _r, int _p) : 
    D(_d), W(_w), particleNum(_p), r(_r), cellsize(cell)
{
    gc.resize(D * W);
    xbound = cellsize * (D - 1);
    ybound = cellsize * (W - 1);
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
    float dt = 0.01;
    float &h = cellsize;
    if (vmax > h / dt)
        dt = h / vmax;

    // advect velocity
    advect(dt);

    // update pressure
    typedef Eigen::SparseMatrix<float, Eigen::RowMajor> MatType;
    MatType A(fluidCells.size(), fluidCells.size());
    typedef Eigen::VectorXf VecType;
    VecType b(fluidCells.size()), x;

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
    cout << "pressure: ";
    for (int i = 0; i < fluidCells.size(); ++i)
    {
        gc[fluidCells[i]].p = x(i);
        cout << x(i) << ", ";
    }
    cout << endl;

    // update velocity
    for (int i = 0; i < D; ++i)
        for (int j = 0 ; j < W; ++j)
        {
            int pos = getIndex(i, j);
            float cur_p = gc[pos].p;
            if (i < D - 1)
                gc[pos].ux += -dt * (gc[getIndex(i + 1, j)].p - cur_p) / r / h;
            else
                gc[pos].ux = 0;
            if (j < W - 1)
                gc[pos].uy += -dt * (gc[getIndex(i, j + 1)].p - cur_p) / r / h;
            else
                gc[pos].uy = 0;

        }

    // update marker particles
    fluidCells.clear();
    for (auto &c : gc)
        c.tid = -1;
    for (auto & i : p)
        i.updatePos(dt, *this);

    t += dt;
    return t;
}

int Grid::getDivergence(int pos) const
{
    return gc[pos + W].ux + gc[pos + 1].uy - gc[pos].ux - gc[pos].uy; 
}

void Grid::advect(float dt)
{
    vector<pair<float, float>> v0;
    v0.resize(D * W);
    int pos;
    /*for (int i = 0; i < D - 1; ++i)
        for (int j = 0; j < W - 1; ++j)
        if (gc[pos = getIndex(i, j)].tid >= 0)
        {
            float cur_p = gc[pos].p;
            gc[pos].vx = gc[pos].ux;
            gc[pos].vy = gc[pos].uy;
            gc[pos].ux += dt * ( g - (gc[getIndex(i + 1, j)].p - cur_p) / r / cellsize);
            gc[pos].uy += -dt * (gc[getIndex(i, j + 1)].p - cur_p) / r / cellsize;
            cout << pos << " " << gc[pos].ux << " " << gc[pos].uy << endl;
        }
*/
    for (int i = 0; i < D - 1; ++i)
        for (int j = 0; j < W - 1; ++j)
        {
            pos = getIndex(i, j);
            float ux = gc[pos].ux, uy = gc[pos].uy;
            float x = i * cellsize - ux * dt, y = j * cellsize - uy * dt;
            x = min(max(0.f, x), xbound);
            y = min(max(0.f, y), ybound);
            int ni = floor(x / cellsize), nj = floor(y / cellsize);

            ux = interpolate(x, ni * cellsize, (ni + 1.0) * cellsize, gc[getIndex(ni, nj)].ux, gc[getIndex(ni + 1, nj)].ux);
            uy = interpolate(y, nj * cellsize, (nj + 1.0) * cellsize, gc[getIndex(ni, nj)].uy, gc[getIndex(ni, nj + 1)].uy);

            if (gc[pos].tid >= 0)
            {
                ux = interpolate(x, ni * cellsize, (ni + 1.0) * cellsize, gc[getIndex(ni, nj)].ux, gc[getIndex(ni + 1, nj)].ux);
                cout << i << " " << j << " " << ni << " " << nj << " " << ux << " " << uy << " ";
                float cur_p = gc[pos].p;
                ux += dt * ( g - (gc[getIndex(i + 1, j)].p - cur_p) / r / cellsize);
                uy += -dt * (gc[getIndex(i, j + 1)].p - cur_p) / r / cellsize;
                cout << pos << " " << ux << " " << uy << endl;
            }

            v0[pos] = pair<float, float>(ux, uy);
        }
    for (int i = 0; i < v0.size(); ++i)
    {
        gc[i].ux = v0[i].first;
        gc[i].uy = v0[i].second;
    }
}

float Grid::interpolate(float x, float lx, float rx, float vl, float vr)
{
    float s = (rx - x) / (rx - lx);
    return s * vl + (1 - s) * vr;
}
