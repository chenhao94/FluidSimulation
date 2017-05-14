# Grid-based Fluid Simulation

In grid-based simulation, we monitor the parameters (velocity and pressure) on the fixed points insteasd of the fluid particles. More specifically, we divide the space in the container into tiny cells in the grid, and record the pressure and velocity on the cell.

## Imcompressible Navier–Stokes Equations

[//]: ![equation](http://bit.ly/2qE93fN)

[//]: ![equation](http://bit.ly/2qEh0BD)

We ignore the viscocity, and assume gravity is the only external force, then

![equation](http://www.sciweavers.org/tex2img.php?eq=%5Cfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20t%7D%20%20%3D%20g%20-%20%5Cfrac%7B%5Cbigtriangledown%20p%7D%7B%5Crho%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

[//]: \frac{\partial u}{\partial t}  = g - \frac{\bigtriangledown p}{\rho}

Thus, if we know the pressure of each cell, we can use it to update the velocity field.

## Semi-Lagrangian Advection

Before solving the pressure, we should update the velocity field first. The velocity field will move with the fluid, so we need to calculate the new velocity field based on the old version. The method we used called *semi-Lagrangian method*. For each point *A* in the new field, we use the velocity of the old field, and go backwards to *B*, and use the velocity of *B* in the old field as the velocity of *A* in the new field.

    foreach point A:
        u = getVelocity(A, old_field);
        B = A - u * dt;
        setVelocity(A, getVelocity(B, old_field), new_field);
        
Then we can apply gravity to the new field.

## Solving the Equations

After updating the velocity, we got *u**. It may violate the incompressible condition: \bigdowntriangle\cdot u = 0. The new pressure values are here to fix it.

[//]: \frac{u^*-u_{old}}{\Delta t}  = g - \frac{\bigtriangledown p}{\rho}

![equation](http://www.sciweavers.org/tex2img.php?eq=%5Cbigtriangledown%20%5Ccdot%20u%5E%2A%20%3D%20%5Cfrac%7B%5CDelta%20t%7D%7B%5Crho%7D%5CDelta%20p%20&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

[//]: \bigtriangledown \cdot u^* = \frac{\Delta t}{\rho}\Delta p 

The boundary conditions are:

* The pressure of the air is zero
* The velocity towards the solid boundary is zero

Then we can solve the pressure can update the velocity:

![equation](http://www.sciweavers.org/tex2img.php?eq=u%20%20%3D%20u%5E%2A%20-%20%5Cfrac%7B%5CDelta%20t%20%5Cbigtriangledown%20p%7D%7B%5Crho%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

[//]: u  = u^* - \frac{\Delta t \bigtriangledown p}{\rho}

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

## References

[1] Bridson, Robert, and Matthias Müller-Fischer. "Fluid simulation: SIGGRAPH 2007 course notes Video files associated with this course are available from the citation page." ACM SIGGRAPH 2007 courses. ACM, 2007.

[2] Braley, Colin, and Adrian Sandu. "Fluid simulation for computer graphics: A tutorial in grid based and particle based methods." Virginia Tech, Blacksburg (2010).
