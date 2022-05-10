# Galaxy Simulation

## Introduction
This project is about the simulation of stars in galaxy. This simulation is generally called 
N-body simulation. The simulation is based on the Newtonian gravitational force wich is

$$F = G\frac{M_1 M_2}{d^2}$$

where `F` is the force, `G` is the gravitational constant, `M1` is the mass of the first body,
`M2` is the mass of the second body and `d` is the distance between the two bodies.

Thanks to that we can calculate the acceleration of the first body in the direction of the second body. The formula is

$$a = \sum \frac{F}{M}$$

where `a` is the acceleration, `F` is the force previous calculated and `M` is the mass of the body.

We can notice that the formula of the acceleration is : 

$$a = \sum \frac{G \frac{M_1 M_2}{d^2}}{M_1}$$

$$a = \sum G\frac{M_2}{d^2}$$

So, the general formula of the acceleration of each body is :

$$a = \sum_{i=1}^{N} G\frac{M_i}{d^2}$$

where `N` is the number of bodies in the galaxy, `G` is the gravitational constant and `M_i` is the mass of the body `i`.