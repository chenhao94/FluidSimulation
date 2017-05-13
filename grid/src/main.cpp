#include <FL/Fl.H>
#include <GL/glut.h>
#include "gridview.hpp"

int main()
{
    GridView view(50, 50, 0.002, 0.05);
    view.show();
    return (Fl::run());
}
