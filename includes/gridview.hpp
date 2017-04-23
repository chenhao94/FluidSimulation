#pragma once

#include "grid.hpp"
#include <FL/Fl_Gl_Window.H>

class GridView : public Fl_Gl_Window, public Grid
{
    public:

        GridView(int d, int w, float cellsize, float dt);
        virtual int handle(int event) override;
        virtual void draw() override;

    private:
        static void timer_cb(void *userdata);
};
