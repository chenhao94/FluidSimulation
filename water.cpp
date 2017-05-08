#include "water.h"

Water::Water() 
{
	size = 0.05;

	stiffness = 0.08;
	n_stiffness = 0.1;

	linear_viscosity = 0.5;
	quadratic_viscosity = 1.0;

	damping = 0.1;
	static_density = 20.0;

	tension = 0.0004;

	max_num = 2000;
}

Water::Water(int max_num)
{
	size = 0.05;

	stiffness = 0.08;
	n_stiffness = 0.1;

	linear_viscosity = 0.5;
	quadratic_viscosity = 1.0;

	damping = 0.1;
	static_density = 20.0;

	tension = 0.0004;

	this->max_num = max_num;
}

void Water::add_particles(std::vector<Particle*> ps_to_add)
{
	int allowd_num = max_num - ps.size();
	if (allowd_num <= 0)
		return;
	ps.insert(ps.end(), ps_to_add.begin(), ps_to_add.end());
}






