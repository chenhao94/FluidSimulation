#include "system.h"
#include <cstdio>
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

void PSystem::compute(Water &water, float dt)
{
	cout << dt << endl;
	gravity(water, dt);
	advect(water, dt);
	this->grid->update(water);
	pressure(water, dt);
	relax_pos(water, dt);
	step(water, dt);
	bound(water, dt);
}

void PSystem::gravity(Water &water, float dt)
{
	cout << "gravity";
	for (int i = 0; i < water.ps.size(); ++i) {
		water.ps[i]->velocity[1] += -9.8f * dt;
	}
	cout << water.ps[0]->velocity[1] << endl;
}
void PSystem::advect(Water &water, float dt)
{
	cout << "advect";
	for (int i = 0; i < water.ps.size(); ++i) {
		water.ps[i]->prePos = water.ps[i]->pos;
		water.ps[i]->pos += water.ps[i]->velocity * dt;
	}
	cout << water.ps[0]->pos[0] << "," << water.ps[0]->pos[1] << endl;
}
void PSystem::bound(Water &water, float dt)
{
	cout << "bound\n";
	for (int i = 0; i < water.ps.size(); ++i) {
		if (water.ps[i]->pos[1] >= this->height){
			water.ps[i]->velocity[1] *= -0.8f;
		} else if (water.ps[i]->pos[1] <= 0) {
			// water.ps[i]->pos[1] += 0.1;
			water.ps[i]->velocity[1] *= -0.8f;
		}
		if (water.ps[i]->pos[0] >= this->width){
			// water.ps[i]->pos[0] -= 0.1;
			water.ps[i]->velocity[0] *= -0.8f;
		} else if (water.ps[i]->pos[0] <= 0) {
			// water.ps[i]->pos[0] += 0.1;
			water.ps[i]->velocity[0] *= -0.8f;
		}
	}
	cout << water.ps[0]->pos[0] << "," << water.ps[0]->pos[1] << endl;
}
void PSystem::pressure(Water &water, float dt)
{
	cout << "pressure\n";
	float den, n_den;
	for (int i = 0; i < water.ps.size(); ++i) {
		Particle *p = water.ps[i];
		pn[i].c = 0;

		den = 0;
		n_den = 0;

		for (int w = p->g_x - 1; w <= p->g_x + 1; w++) {
			for (int h = p->g_y - 1; h <= p->g_y + 1; h++) {
				// if (i == 0) printf("%d, %d\n", w, h);
				Particle * vic = this->grid->grid[w + h * this->grid->grid_width];
				while (vic != NULL) {
					float dis = (p->pos - vic->pos).length();
					// if (i == 0) printf("%f, %d\n", dis * dis, w + h * this->grid->grid_width);
					if (dis >= 0.0001 && dis <= this->cell_size) {
						float co = 1 - (dis / this->cell_size);
						den += vic->mass * co * co * this->kernel;
						n_den += vic->mass * co * co * co * this->n_kernel;
						if (pn[i].c < MAX_NBR) {
							int idx = pn[i].c;
							pn[i].nbr[idx] = vic;
							pn[i].dis[idx] = dis;
							pn[i].c++;
						}
					}
					vic = vic->next;
				}
				// if (i == 0)printf("\n");
			}
		}

		p->density = den;
		p->n_density = n_den;
		p->pressure = water.stiffness * (p->density - water.static_density);
		p->n_pressure = water.n_stiffness * p->n_density;
		if (i == 0)
			printf("%f, %f, %f, %f\n", p->density, p->n_density, p->pressure, p->n_pressure);
	}
	cout << water.ps[0]->pos[0] << "," << water.ps[0]->pos[1] << endl;
}
void PSystem::relax_pos(Water &water, float dt)
{
	cout << "relex_pos\n";
	for (int i = 0; i < water.ps.size(); ++i) {
		Particle *p = water.ps[i];
		Vec2f r_pos = p->pos;
		// cout << r_pos[0] << "," << r_pos[1] << endl;
		for (int j = 0; j < pn[i].c; j++) {
			Vec2f dxy = pn[i].nbr[j]->pos - p->pos;
			float r = pn[i].dis[j];
			float co = 1.0 - (r / this->cell_size);

			float d = 0.5 * (dt * dt) * ((pn[i].nbr[j]->n_pressure + p->pressure) * (co * co) * this->n_kernel 
					+ (pn[i].nbr[j]->pressure + p->pressure) * co * this->kernel);

			r_pos -= (d * dxy) / (r * p->mass);
			// if (i == 0) printf("m:%d, %f, %f , %f, %f, %f, %f, %f\n",
			// 					 j, r_pos[0], r_pos[1], dxy[0], dxy[1], d, r, p->mass);
			// if (j == 0) cout << "1st cut: "<< r_pos[0] << "," << r_pos[1] << endl;
			if (co < 1) {
				float v2 = (pn[i].nbr[j]->velocity - p->velocity) * dxy;
				if (v2 > 0) {
					v2 /= r;
					float f = 0.5 * dt * co;
					f *= (water.linear_viscosity * v2 + water.quadratic_viscosity * v2 * v2);
					dxy *= (f * dt);
					r_pos -= dxy;
					// if (j == 0) cout <<"2nd cut: "<< r_pos[0] << "," << r_pos[1] << endl;
				}
			}
		}
		p->relPos = r_pos;
	}
	cout << water.ps[0]->relPos[0] << "," << water.ps[0]->relPos[1] << endl;
	cout << water.ps[0]->pos[0] << "," << water.ps[0]->pos[1] << endl;
}
void PSystem::step(Water &water, float dt)
{
	cout << "step\n";
	for (int i = 0; i < water.ps.size(); ++i) {
		water.ps[i]->pos = water.ps[i]->relPos;
		water.ps[i]->velocity = (water.ps[i]->pos - water.ps[i]->prePos) / dt;
	}
	cout << water.ps[0]->pos[0] << "," << water.ps[0]->pos[1] << endl;
}
