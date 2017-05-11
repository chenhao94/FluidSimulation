#
#  Makefile for fltk applications
#

LOCAL = /usr/local

INCLUDE = -I$(LOCAL)/include
LIBDIR = -L$(LOCAL)/lib

LDLIBS = -L/usr/local/lib/ -framework Carbon -framework ApplicationServices -framework Cocoa
GLDLIBS = -framework AGL -framework OpenGL -framework GLUT
LIBS  = $(LDLIBS) $(GLDLIBS) -lfltk_gl -lfltk -lfltk_images -lfltk_forms -lfltk_jpeg -lpng -lm -lglfw -lglew

CFLAGS = -Ig
All:
	g++ main.cpp particle.cpp water.cpp system.cpp grid.cpp -o sph $(INCLUDE) $(LIBDIR) $(LIBS)