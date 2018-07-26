A program for visualizing forces between particles.

In its current state, we can visualize the motion of one particle. Still need to solve problems such as handling interactions between many particles. A solution to this is creating a global array of physical potential or forces. Self interactions are eliminated by subtracting it's own potential field from the global array, then calculating the interactions with the remaining field.

The latest update has two particles. However, the handling of forces is a little clunky. A conversion is needed for positions, velocities, and accelerations to be more precise. Then, at the last stage should be to round position to an unsigned integer so it can be displayed on the screen.
