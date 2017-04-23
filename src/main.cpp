#include <FL/Fl.H>
#include <GL/glut.h>
#include "gridview.hpp"

int main()
{
    GridView view(20, 20, 0.01, 0.05);
    view.show();
    return (Fl::run());
}
