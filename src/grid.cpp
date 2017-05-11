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

    // advect velocity and pressure
    advect(dt);

    // update pressure
    typedef Eigen::SparseMatrix<float, Eigen::RowMajor> MatType;
    MatType A(fluidCells.size(), fluidCells.size());
    typedef Eigen::VectorXf VecType;
    VecType b(fluidCells.size()), x;
    static constexpr int d[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int i = 0; i < D - 1; ++i)
        for (int j = 0; j < W - 1; ++j)
        {
            int pos = getIndex(i, j), tpos, id, tid;
            if ((id = gc[pos].tid) < 0)
                gc[pos].p = 0;
            else
            {
                int omega = 0;
                for (int k = 0; k < 4; ++k)
                {
                    int x = i + d[k][0], y = j + d[k][1];
                    if (0 < x && x < D - 1 && 0 < y && y < W - 1)
                    {
                        ++omega;
                        tpos = getIndex(x, y);
                        if ((tid = gc[tpos].tid) >= 0)
                        {
                            A.insert(id, tid) = - dt / r / h / h;
                        }
                    }
                }
                A.insert(id, id) = omega * dt / r / h / h;
                b(id) = -getDivergence(pos); 
            }
        }
    Eigen::BiCGSTAB<MatType> solver;
    x = solver.compute(A).solve(b);
    for (int i = 0; i < fluidCells.size(); ++i)
        gc[fluidCells[i]].p = x(i);

    // update velocity
    for (int i = 1; i < D; ++i)
        for (int j = 1 ; j < W; ++j)
        {
            int pos = getIndex(i, j);
            float cur_p = gc[pos].p;
            if (i < D - 2)
                gc[pos].ux -= dt * (gc[getIndex(i + 1, j)].p - cur_p) / r / h;
            else
                gc[pos].ux = 0;
            if (j < W - 2)
                gc[pos].uy -= dt * (gc[getIndex(i, j + 1)].p - cur_p) / r / h;
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

float Grid::getDivergence(int pos) const
{
    return (gc[pos].ux + gc[pos].uy - gc[pos - W].ux - gc[pos - 1].uy) / cellsize; 
}

void Grid::advect(float dt)
{
    vector<pair<float, float>> v0, v;
    vector<float> p;
    v0.resize(D * W);
    v.resize(D * W);
    p.resize(D * W);
    int pos;
    for (int i = 1; i < D - 1; ++i)
        for (int j = 1; j < W - 1; ++j)
        {
            pos = getIndex(i, j);
            float vx = gc[pos].ux, vy = gc[pos].uy;
            if (i > 0)
                vx += gc[getIndex(i-1, j)].ux;
            if (j > 0)
                vy += gc[getIndex(i, j-1)].uy;
            v0[pos] = pair<float, float>(0.5 * vx, 0.5 * vy);
        }
    for (int i = 1; i < D - 1; ++i)
        for (int j = 1; j < W - 1; ++j)
        {
            pos = getIndex(i, j);
            gc[pos].vx = gc[pos].ux;
            gc[pos].vy = gc[pos].uy;
            float x = (i + 0.5) * cellsize - v0[pos].first * dt;
            float y = (j + 0.5) * cellsize - v0[pos].second * dt;
            x = min(max(0.5f * cellsize, x), xbound - 0.5f * cellsize);
            y = min(max(0.5f * cellsize, y), ybound - 0.5f * cellsize);
            int ni = floor(x / cellsize - 0.5), nj = floor(y / cellsize - 0.5);
            float ux = (ni + 0.5) * cellsize, dx = (ni + 1.5) * cellsize;
            float ly = (nj + 0.5) * cellsize, ry = (nj + 1.5) * cellsize;
            int ul = getIndex(ni, nj), ur = getIndex(ni, nj + 1);
            int dl = getIndex(ni + 1, nj), dr = getIndex(ni + 1, nj + 1);
            p[pos] = bilinearInterpolate(x, y, ux, ly, dx, ry, gc[ul].p, gc[ur].p, gc[dl].p, gc[dr].p);
            v[pos] = pair<float, float>(bilinearInterpolate(x, y, ux, ly, dx, ry, gc[ul].ux, gc[ur].ux, gc[dl].ux, gc[dr].ux),
                                        bilinearInterpolate(x, y, ux, ly, dx, ry, gc[ul].uy, gc[ur].uy, gc[dl].uy, gc[dr].uy));
        }

    for (int i = 1; i < D - 1; ++i)
        for (int j = 1; j < W - 1; ++j)
        {
            pos = getIndex(i, j);
            gc[pos].p = p[pos];
            gc[pos].ux = v[pos].first;
            gc[pos].uy = v[pos].second;
            if (i < D - 2)
                gc[pos].ux += v[getIndex(i + 1, j)].first;
            if (j < W - 2)
                gc[pos].uy += v[getIndex(i, j + 1)].second;
            gc[pos].ux *= 0.5;
            gc[pos].uy *= 0.5;
        }

    for (int i = 1; i < D - 1; ++i)
        for (int j = 1; j < W - 1; ++j)
        {
            pos = getIndex(i, j);
            if (gc[pos].tid >= 0)
            {
                gc[pos].ux += g * dt;
                if (i == D - 2 && gc[pos].ux > 0)
                    gc[pos].ux = 0;
                if (j == W - 2 && gc[pos].uy > 0)
                    gc[pos].uy = 0;
            }
            else
                gc[pos].ux = gc[pos].uy = 0;
        }
    for (int i = 1; i < D - 1; ++i)
        for (int j = 1; j < W - 1; ++j)
        {
            pos = getIndex(i, j);
            if (gc[pos].tid < 0)
            {
                if (gc[getIndex(i-1, j)].tid >=0)
                    gc[pos].ux = gc[getIndex(i-1, j)].ux;
                if (gc[getIndex(i+1, j)].tid >=0)
                    gc[pos].ux = gc[getIndex(i+1, j)].ux;
                if (gc[getIndex(i, j-1)].tid >=0)
                    gc[pos].uy = gc[getIndex(i, j-1)].uy;
                if (gc[getIndex(i, j+1)].tid >=0)
                    gc[pos].uy = gc[getIndex(i, j+1)].uy;
            }
        }
}

float Grid::bilinearInterpolate(float x, float y, float ux, float ly, float dx, float ry, float vul, float vur, float vdl, float vdr)
{
    float s = (dx - ux) * (ry - ly);
    float a = (x - ux) * (y - ly) / s;
    float b = (dx - x) * (y - ly) / s;
    float c = (x - ux) * (ry - y) / s;
    float d = (dx - x) * (ry - y) / s;
    if (!(a > -1e-4 && b > -1e-4 && c > -1e-4 && d > -1e-4 && fabs(a+b+c+d-1.0) < 1e-4))
    {
        std::cerr << "error!" << a << " " << b << " " << c << " " << d <<std::endl;
        exit(-1);
    }
    return vul * d + vur * b + vdl * c + vdr * a;
}
