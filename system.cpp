#include "system.h"
#define PI 3.1415926


PSystem::PSystem(float size, int w, int h, int win_w, int win_h)
{
	this->grid = new Grid(size, w, h);
	this->width = win_w;
	this->height = win_h;
	this->cell_size = size;

	kernel = 10.f/(PI*this->cell_size*this->cell_size);
	n_kernel = 15.f/(PI*this->cell_size*this->cell_size);
}

void PSystem::gravity(Water &water, float dt)
{
	for (int i = 0; i < water.ps.size(); ++i) {
		water.ps[i]->velosity[1] += -9.8f * dt;
	}
}
void PSystem::advect(Water &water, float dt)
{
	for (int i = 0; i < water.ps.size(); ++i) {
		water.ps[i]->prePos = water.ps[i]->pos;
		water.ps[i]->pos = water.ps[i]->velosity * dt;
	}
}
void PSystem::bound(Water &water, float dt)
{
	for (int i = 0; i < water.ps.size(); ++i) {
		if (water.ps->pos[1] >= this->height){
			water.ps->velosity[1] *= -0.8f;
		} else if (water.ps->pos[1] <= 0) {
			water.ps->velosity[1] *= -0.8f;
		}
		if (water.ps->pos[0] >= this->width){
			water.ps->velosity[0] *= -0.8f;
		} else if (water.ps->pos[0] <= 0) {
			water.ps->velosity[0] *= -0.8f;
		}
	}
}
void PSystem::pressure(Water &water, float dt)
{
	float den, n_den;
	for (int i = 0; i < water.ps.size(); ++i) {
		Particle *p = water.ps[i];
		pn[i].c = 0;

		den = 0;
		n_den = 0;
		for (int w = p->g_x - 1; w < p->g_x + 1; w++) {
			for (int h = p->g_y - 1; h < p->g_y + 1; h++) {
				Particle * vic = this->grid->grid[w + h * this->grid->grid_width];
				while (vic != NULL) {
					float dis = (p->pos - vic->pos).length();
					if (dis > 0.0001 || dis < this->cell_size) {
						float co = 1 - (dis / this->cell_size);
						den += vic->mass * co * co * this->kernel;
						n_den += vic->mass * co * co * this->n_kernel;
						if (pn.c < MAX_NBR) {
							pn[i].nbr.push_back(vic);
							pn[i].dis.push_back(dis);
							pn[i].c++;
						}
					}
				}
			}
		}

		p->density = den;
		p->n_density = n_den;
		p->pressure = water.stiffness * (pi->density - water.static_density);
	}
}
	void relax_pos(Water &water, float dt);
	void step(Water &water, float dt);
