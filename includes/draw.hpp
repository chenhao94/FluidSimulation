#pragma once

#include <GL/glut.h>

inline void drawRectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);          
    glVertex2f(x1, y1);    
    glVertex2f(x1, y2);    
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
}

inline void drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1, y1);    
    glVertex2f(x2, y2);
    glEnd();
}

inline void drawPoint(float x, float y)
{
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}
