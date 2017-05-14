# Grid-based Fluid Simulation

In grid-based simulation, we monitor the parameters (velocity and pressure) on the fixed points insteasd of the fluid particles. More specifically, we divide the space in the container into tiny cells in the grid, and record the pressure and velocity on the cell.

## Imcompressible Navier–Stokes Equations

We ignore the viscocity, and assume gravity is the only external force, then

![equation](https://raw.githubusercontent.com/chenhao94/FluidSimulation/master/grid/img/f0.png)

Thus, if we know the pressure of each cell, we can use it to update the velocity field.

## Semi-Lagrangian Advection

Before solving the pressure, we should update the velocity field first. The velocity field will move with the fluid, so we need to calculate the new velocity field based on the old version. The method we used called *semi-Lagrangian method*. For each point *A* in the new field, we use the velocity of the old field, and go backwards to *B*, and use the velocity of *B* in the old field as the velocity of *A* in the new field.

    foreach point A:
        u = getVelocity(A, old_field);
        B = A - u * dt;
        setVelocity(A, getVelocity(B, old_field), new_field);
        
Then we can apply gravity to the new field.

## Solving the Equations

After updating the velocity, we got *u**. It may violate the incompressible condition: ![equation](https://raw.githubusercontent.com/chenhao94/FluidSimulation/master/grid/img/f0_5.png). The new pressure values are here to fix it.

![equation](https://raw.githubusercontent.com/chenhao94/FluidSimulation/master/grid/img/f1.png)

The boundary conditions are:

* The pressure of the air is zero
* The velocity towards the solid boundary is zero

Then we can solve the pressure can update the velocity:

![equation](https://raw.githubusercontent.com/chenhao94/FluidSimulation/master/grid/img/f2.png)

## Marker Particles

We use the *marker particles* to mark whether a cell is fluid or air. In this case, if a cell contains any particle, it will be regarded as fluid, otherwise it is air.

## Pseudo Code

    foreach step:
        advect(velocity);
        apply(velocity, gravity);
        solve_pressure();
        apply(velocity, pressure);
        update_particles();
        
## Artifact

<img src="https://raw.githubusercontent.com/chenhao94/FluidSimulation/master/grid/img/a1.png", width="500", height="500">

<img src="https://raw.githubusercontent.com/chenhao94/FluidSimulation/master/grid/img/a2.png", width="500", height="500">

## References

[1] Bridson, Robert, and Matthias Müller-Fischer. "Fluid simulation: SIGGRAPH 2007 course notes Video files associated with this course are available from the citation page." ACM SIGGRAPH 2007 courses. ACM, 2007.

[2] Braley, Colin, and Adrian Sandu. "Fluid simulation for computer graphics: A tutorial in grid based and particle based methods." Virginia Tech, Blacksburg (2010).
