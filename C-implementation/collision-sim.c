#include <stdio.h>
#include <math.h>
#include <stdbool.h>

struct Object {
	double m;
	double x;
	double v;
	double l;
};

int check_collisions(double x1, double x2) {

	// Only 4 cases to deal with (0-3)
	int collision_type = 0; // type 0 for no collision

	if (x1 <= 0) {
		collision_type += 1; // type 1 for collision with wall
	}
	if (x1 >= x2) {
		collision_type += 2; // type 2 for collision between bodies
	}

	// if both collisions occur collision_type = 3

	return collision_type;
}

double update_v(double u1, double u2, double m1, double m2) {
	double v1 = (m1-m2)/(m1+m2)*u1 + (2*m2)/(m1+m2)*u2;
	return v1;
}

void run_sim()
{
	// Simulation settings
	double t = 0; // keep track of time
	double dt = pow(10, -8);
	double tmax = 30; // length of simulation
	int n = 6; // order of magnitude of collision

	// Create sliding objects
	struct Object small;
	struct Object large;

	// Set initial conditions
	int x = 2*(n-1);
	int im = pow(10, x);
	double fm = im;
	small.m = 1.0; small.x = 0.3; small.v =  0.0;
	large.m =  fm; large.x = 1.0; large.v = -0.1;

	// Initialise a counter for the collisions
	int collision_counter = 0;
	int collision_type;

	// Start simulation
	while (t < tmax) {

		// Increment time
		t = t + dt;

		// Move objects
		small.x += small.v * dt;
		large.x += large.v * dt;

		// Check for collisions
		collision_type = check_collisions(small.x, large.x);

		// Deal with collisions
		if (collision_type == 0) {
			;
		} else if (collision_type == 1) {
			small.v = ( -1 * small.v ); // reverse Object's direction
			collision_counter++;
		} else if (collision_type == 2) {
			double u1 = small.v; // adjust velocity of both objects
			double u2 = large.v; // following conservation of p and E
			small.v = update_v(u1, u2, small.m, large.m);
			large.v = update_v(u2, u1, large.m, small.m);
			collision_counter++;
		} else {
			printf("The simulation failed because the temporal resolution is too low to resolve individual collisions.\n");
			break;
		}

	}

	printf("The simulation counted %d collisions.\n", collision_counter);
}

int main() {

	run_sim();

	return 0;
}
