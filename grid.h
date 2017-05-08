#ifndef __GRID_H__
#define __GRID_H__

#include "particle.h"
#include "water.h"
#include <vector>
using namespace std;

class Grid
{
public:
	Grid(float cell_size, int w, int h);
	update(Water wtr);
	~Grid();
	
	float cell_size;
	int grid_width;
	int grid_hright;
	int cell_count;
	vector<Particle*> grid;
};

#endif