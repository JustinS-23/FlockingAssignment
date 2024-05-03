# FlockingAssignment

## NOTE:
### To start the program, be sure to compile the code before running the simulation. Otherwise,
### the boids won't appear. Also, if you open the project after compiling and don't see the agents, 
### then there may be an error with the GMB_BP. This is likely caused by live coding from UE5, 
### if this is the case, then you'll likely need to remake the GMB_BP and re-add the static mesh. 
### I apologize for the inconvenience, I tried finding work-arounds but disabling live coding 
### was the only solution I could find that worked for me.


## Regarding project contents:
### For the project itself, I added a couple small tweaks. The most noticeable is that I created
### a boundary box for the boids that it cannot leave. The less noticeable addition was that the
### boids start off with a random direction and velocity, however they quickly flock together and
### their velocities become very close to one another. If there is some weird movement, then resetting
### the simulation should fix it since they start at random directions and positions.