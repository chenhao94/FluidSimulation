#include "gridview.hpp"
#include "particle.hpp"
#include "draw.hpp"
#include <FL/Fl.H>
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <iostream>

static unsigned cellPixelSize = 10;

GridView::GridView(int d, int w, float cellsize, float _dt) : Fl_Gl_Window(w * cellPixelSize, d * cellPixelSize), Grid(d, w, cellsize), dt(_dt)
{
    Fl::add_timeout(0.1, timer_cb, (void*)this);
}

void GridView::timer_cb(void *userdata)
{
    ((GridView*)userdata)->redraw();
    Fl::repeat_timeout(0.1, timer_cb, userdata);
}

int GridView::handle(int event)
{
    using namespace std;
    auto x = Fl::event_x() / cellPixelSize, y = Fl::event_y() / cellPixelSize;
    static unsigned sx, sy;
    auto button = Fl::event_button();
    if (event == FL_PUSH && button == FL_LEFT_MOUSE)
    {
        sx = x;
        sy = y;
    }
    else if (event == FL_RELEASE)
    {
        for (auto i = max(min(sy, y), 1u); i <= min(max(sy, y), D - 2u); ++i)
            for (auto j = max(min(sx, x), 1u); j <= min(max(sx, x), W - 2u); ++j)
                addFluid(i, j);
    }
    return 1;
}

void GridView::draw()
{
    using namespace std;
    auto nextT = 1. * ceil(t / dt) * dt;
    while (step() < nextT);
    if (t - nextT < 5e-4)
        t += 5e-4;

    glClear(GL_COLOR_BUFFER_BIT);
 
    for (int i = 0; i < D; ++i)
        for (int j = 0; j < W; ++j)
        {
            if (i == 0 || i == D - 1 || j == 0 || j == W - 1)
                glColor3f(0.2f, 0.2f, 0.2f);
            else if (gc[getIndex(i, j)].tid < 0)
                glColor3f(0.5f, 0.5f, 0.5f);
            else
                glColor3f(0.f, 0.5f, 0.5f);
            drawRectangle(2. * j / W - 1.  , 1. - 2. * i / D,
                    2. * (j + 1) / W - 1., 1. - 2. * (i + 1) / D);
        }


    glColor3f(0.f, 0.f, 0.f);
    for (int i = 1; i < D; ++i)
        drawLine(-1., 1. - 2. * i / D , 1, 1. - 2. * i / D);

    for (int i = 1; i < W; ++i)
        drawLine(2. * i / W - 1., -1. ,2. * i / W - 1., 1.);

    glColor3f(1.f, 1.f, 1.f);
    for (auto & i : p)
        drawPoint(2. * i.y / cellsize / W - 1., 1. - 2 * i.x / cellsize/ D);

    glFinish(); 
}
