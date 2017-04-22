#pragma once
#include <vector>

struct GridCell
{
    float ux, uy, vx, vy, px, py;
    int tid;
};


class Grid
{
    public:

        Grid(int _d, int _w, float cell, float _r = 1000.0);

        int getD() const { return d; }
        int getW() const { return w; }
        float getCellSize() const { return cellsize; }
        const GridCell& getGridCell(int pos) const { return g[pos];  }
        const GridCell& getGridCell(int x, int y) const { return getGridCell(getIndex(x, y)); }

        void reset();
        void addFluid(int x, int y);
        void markFluid(int x, int y); { g[getIndex(x, y)].fluid = true; }

        // return the new timestamp, put the density info in `buf`
        float step(float *buf);

        int getIndex(int x, int y) const { return x * w + y; }

    private:

        int d, w;
        float r;
        float t, cellsize;

        std::vector<GridCell> g;

        static std::vector<int> fluidCells;
};

