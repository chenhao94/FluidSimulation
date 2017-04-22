#pragma once
#include <vector>

class Particle;

struct GridCell
{
    float ux, uy, vx, vy, p;
    int tid;
};


class Grid
{
    public:

        Grid(int _d, int _w, float cell, float _r = 1000.0, int _p = 2);

        int getD() const { return d; }
        int getW() const { return w; }
        float getCellSize() const { return cellsize; }
        const GridCell& getGridCell(int pos) const { return gc[pos];  }
        const GridCell& getGridCell(int x, int y) const { return getGridCell(getIndex(x, y)); }

        void reset();
        void addFluid(int x, int y);
        void markFluid(int x, int y); 

        // return the new timestamp, put the density info in `buf`
        float step(float *buf);

        int getIndex(int x, int y) const { return x * w + y; }
        int getDivergence(int pos) const;
        int getDivergence(int x, int y) const { return getDivergence(getIndex(x ,y)); };

    private:

        int d, w, particleNum; // d - depth, w - width, including bounds
        float r;
        float t, cellsize;

        std::vector<GridCell> gc;
        static std::vector<int> fluidCells;
        std::vector<Particle> p;
};

