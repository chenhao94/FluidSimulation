# Particle-based Fluid Simulation

## Simulation with particles

The concept of simulation with particles is similar to the particle system. Here, our fluid is represented by particles which is evolving accoding to the space and time. For each iteration and each particle, we need calculate all of the potential forces applied on each particle and then we need to update the velocity. Finally, we'll use the velocity to update the particle position. 

## PseudoCode

	for each particle i:
		- apply gravity
		- advect according to velocity
		- calculate pressure and viscosity for displacement
		- add displacement
		- calculate the new velocity
		- solve collisions with bound

## Some tricks

- For neighbor search of each particle, we assign a hash grid with the side-length of *h* (interaction radius) for each particle. Hence, for a 2D simulation, to search the particle, we just need to search the particles in the adjacent 8 grids.	

## Artifacts

<iframe width="560" height="315" src="https://www.youtube.com/embed/Xnz5Ma3nVU0" frameborder="0" allowfullscreen></iframe>

## Reference

-	Clavet, Simon, Philippe Beaudoin, and Pierre Poulin. "[Particle-based viscoelastic fluid simulation](https://www.researchgate.net/publication/220789321_Particle-based_viscoelastic_fluid_simulation)." Proceedings of the 2005 ACM SIGGRAPH/Eurographics symposium on Computer animation. ACM, 2005.

- Braley, Colin, and Adrian Sandu. "[Fluid simulation for computer graphics: A tutorial in grid based and particle based methods](https://pdfs.semanticscholar.org/0b52/1fdd0b92e00b221012374cbaa70fe163feea.pdf)." Virginia Tech, Blacksburg (2010).
		
		