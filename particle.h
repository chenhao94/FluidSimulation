#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <vector>
#include "vec.h"
#include <glm/glm.hpp>
#define MAX_NBR 64

class Particle
{
public:
	Particle() : pos(Vec2f(0,0)), prePos(Vec2f(0,0)), relPos(Vec2f(0,0)),
				velocity(Vec2f(0,0)), pressure(0), n_pressure(0),
				density(0), n_density(0), mass(0), g_x(0), g_y(0)
				{};
	~Particle();

	Vec2f pos;
	Vec2f prePos;
	Vec2f relPos;
	Vec2f velocity;

	float pressure;
	float n_pressure;

	float density;
	float n_density;

	float mass;

	int g_x;
	int g_y;

	Particle* next;
};

class ParticleNeighbor
{
public:
	ParticleNeighbor(): nbr(MAX_NBR), dis(MAX_NBR), c(0) 
						{};
	~ParticleNeighbor();
	
	vector<Particle*> nbr;
	vector<float> dis;
	int c;
};



#endif