#pragma once

struct GridCell
{
    float ux, uy, vx, vy, px, py, r;

    GridCell(float ra = 0.0) : r(ra) {}
};


class Grid
{
    public:

        Grid(int _d, int _w, float _rf = 1000.0, float _ra = 1.225 ) : d(_d), w(_w), rf(_rf), ra(_ra)
        {
            g = new GridCell[d * w];
        }

        ~Grid() { delete[] g; }

        int getD() const { return d; }
        int getW() const { return w; }
        float getCellSize() const { return cellsize; }
        const GridCell& getGridCell(int pos) const { return g[pos];  }
        const GridCell& getGridCell(int x, int y) const { return getGridCell(getIndex(x, y)); }

        void reset();
        void addFluid(int x, int y);

        // return the new timestamp, put the density info in `buf`
        float step(float *buf);

        int getIndex(int x, int y) const { return x * w + y; }

    private:

        int d, w;
        float rf, ra;
        float t, cellsize;

        GridCell *g;
};

