#include "grid.h"

Grid::Grid(float cell_size, int w, int h)
{
	this->cell_size = cell_size;
	this->grid_width = w;
	this->grid_height = h;
	grid.clear();
	grid.resize(w * h);
}

void Grid::update(Water wtr)
{
	grid.clear()
	std::vector<Particle*> pv = wtr.ps;

	for (int i = 0; i < pv.size(); ++i) {
		
		int x = pv[i]->pos[0] / this->cell_size;
		//refine
		if (x < 1) {
			x = 1;
		}
		else if (x > this->grid_width - 2) {
			x = this->grid_width - 2;
		}
		int y = pv[i]->pos[1] / this->cell_size;
		if (y < 1) {
			y = 1;
		} else if (y > this->grid_height - 2) {
			y = this->grid_height - 2;
		}
		//build the linked list
		pv[i]->next = grid[y * this->grid_width + x];
		grid[y * this->grid_width + x] = pv[i];

		pv[i]->g_x = x;
		pv[i]->g_y = y;
	}
}