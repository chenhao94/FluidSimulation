#ifndef __PSYSTEM_H__
#define __PSYSTEM_H__

#include "grid.h"
#include "particle.h"

class PSystem
{
public:
	PSystem(float size, int w, int h, int win_w, int win_h);
	~PSystem();
	
	Grid * grid;
	float cell_size;
	float width;
	float height;
	float kernel;
	float n_kernel;
	vector<ParticleNeighbor> pn;

	void simulate(Water &water, float dt);
	void gravity(Water &water, float dt);
	void advect(Water &water, float dt);
	void bound(Water &water, float dt);
	void pressure(Water &water, float dt);
	void relax_pos(Water &water, float dt);
	void step(Water &water, float dt);
};

#endif