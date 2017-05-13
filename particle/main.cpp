#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include "water.h"
#include "system.h"
#include "particle.h"
#include <time.h>
 
void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

using namespace std;

int main(int argc, char const *argv[])
{
	if (!glfwInit()) {
		cout << "Failed to init GLFW\n";
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(640, 400, "Particles", NULL, NULL);
	cout << window << endl;

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	glClearColor(0.f, 0.f, 0.f, 1.f);

	Water * wt = new Water(5000);

	PSystem * ps = new PSystem(0.3, 10.0/0.3,  5.0/ 0.3, 10.0, 5.0);
	vector<Particle*> particles;
	for (int i = 0; i < 1500; ++i) {
		cout << "Add [" << i<< "]\n";
		Particle* p = new Particle();
		// p->pos[0] = 0.1 * (i % 50);
		// p->pos[1] = 0.2 * (i / 50);
		p->pos = Vec2f((float)rand()/RAND_MAX * 8 + 1, (float)rand()/RAND_MAX * 2 + 1);
		// printf("%f, %f\n", p->pos[0], p->pos[1]);
		p->prePos = p->pos;
		p->mass = 0.5f;
		p->velocity = Vec2f(0,0);
		particles.push_back(p);
	}

	wt->add_particles(particles);
	int frame = 0;

	while (!glfwWindowShouldClose(window)) {
		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) 
			break;
		frame++;
		//sleep(2000);
		ps->compute(*wt, 0.004);
		// ps->compute(*wt, 0.002);
		// break;
		cout << "frame: " << frame << " (" << wt->ps[0]->pos[0] << ", "<< wt->ps[0]->pos[1]<< endl;
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.f, 0.f, 0.f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 10, 0, 5, 0, 1);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		vector<Particle*> particles = wt->ps;
		cout << particles.size() << "  " << particles[0]->pos[0] << endl;
		vector<glm::vec2> positions;
		vector<glm::vec4> colors;

		for(int i = 0; i < particles.size(); ++i) {
			Particle* pi = particles[i];
			//printf("[%f, %f].", pi->pos[0], pi->pos[1]);
			//if (i % 5 == 0) printf("\n");
			positions.push_back(glm::vec2(pi->pos[0], pi->pos[1]));
			glm::vec2 sd = glm::normalize(glm::vec2(pi->velocity[0], pi->velocity[1]));
			// colors.push_back(glm::vec4(sd, 1.f, 0.8f));
			colors.push_back(glm::vec4(0.2, 0.5, 1.f, 0.8f));
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glPointSize(12);

		glColorPointer(4, GL_FLOAT, sizeof(glm::vec4), &colors[0]);
		glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), &positions[0]);
		glDrawArrays(GL_POINTS, 0, particles.size());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glfwSwapBuffers(window);
		glfwPollEvents();


		if (frame == 500) {
			vector<Particle*> drop;
			for (int i = 0; i < 50; ++i) {
				cout << "Add [" << i<< "]\n";
				Particle* p = new Particle();

				p->pos = Vec2f((float)rand()/RAND_MAX * 1 + 4, (float)rand()/RAND_MAX * 1 + 5);
				// printf("%f, %f\n", p->pos[0], p->pos[1]);
				p->prePos = p->pos;
				p->mass = 0.5f;
				p->velocity = Vec2f(0,0);
				drop.push_back(p);
			}
			wt->add_particles(drop);
		}
	}

	glfwTerminate();

	return 0;
}