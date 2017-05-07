#pragma once
#include <vector>
#include "particle.hpp"

struct GridCell
{
    float ux, uy, vx, vy, p;
    int tid;

    GridCell() : tid(-1) {}
};


class Grid
{
    public:

        Grid(int _d, int _w, float cell, float _r = 1000.0, int _p = 2);

        int getD() const { return D; }
        int getW() const { return W; }
        float getCellSize() const { return cellsize; }
        const GridCell& getGridCell(int pos) const { return gc[pos];  }
        const GridCell& getGridCell(int x, int y) const { return getGridCell(getIndex(x, y)); }

        void reset();
        void addFluid(int x, int y);
        void markFluid(int x, int y); 
        bool isFluid(int x, int y) const { return gc[getIndex(x, y)].tid >= 0; }

        // return the new timestamp, put the density info in `buf`
        float step();

        int getIndex(int x, int y) const { return x * W + y; }
        int getDivergence(int pos) const;
        int getDivergence(int x, int y) const { return getDivergence(getIndex(x ,y)); };

        const std::vector<Particle>& getParticles() const { return p; }

        static float xbound, ybound;
        static float bilinearInterpolate(float x, float y, float ux, float ly, float dx, float ry, float vul, float vur, float vdl, float vdr);

    protected:

        int D, W, particleNum; // d - depth, w - width
        float r;
        float t, cellsize;

        std::vector<GridCell> gc;
        static std::vector<int> fluidCells;
        std::vector<Particle> p;

        void advect(float dt);
};

