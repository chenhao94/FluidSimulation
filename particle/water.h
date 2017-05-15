#ifndef __WATER_H__
#define __WATER_H__ 

#include <vector>
#include <glm/glm.hpp>
#include "vec.h"
#include "particle.h"

class Water
{
public:
	Water();
	Water(int max_num);

	~Water();

	float size;
	float stiffness;
	float n_stiffness;

	float linear_viscosity;
	float quadratic_viscosity;

	float damping;
	float static_density;

	float tension;

	vector<Particle*> ps;
	int max_num;

	void add_particles(std::vector<Particle*> ps_to_add);
};


#endif