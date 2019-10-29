#include <stdio.h>
#include <math.h>
#include <stdbool.h>

struct Body {
	double m;
	double x;
	double v;
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

/*double update_v(double u1, double u2, double m1, double m2) {
	double v1 = (m1-m2)/(m1+m2)*u1 + (2*m2)/(m1+m2)*u2;
	return v1;
}*/

void p_transfer(struct Body *p1, struct Body *p2) {

	// store first velocity temporarily
	double u1 = p1->v;

	// update velocities
	p1->v = (p1->m - p2->m)/(p1->m + p2->m)*u1 + (2*p2->m)/(p1->m + p2->m)*p2->v;
	p2->v = (2*p1->m)/(p1->m + p2->m)*u1 + (p2->m - p1->m)/(p1->m + p2->m)*p2->v;
}

bool is_finished(double v1, double v2) {

	bool finished;

	// simulation is finished if no more collisions can occur
	if (v1>0 && v2>v1) { //simplified this expression
		finished = true;
	} else {
		finished = false;
	}

	return finished;
}

void run_sim(int n, double v_init)
{
	/* No longer use time to control resolution of simulation
	therefore reducing number of variables to keep track of
	and increment. Now only checks it is finished every 10000
	cycles as the check is somewhat demanding. Reduced starting
	distance between the two bodies as time before initial
	collision contributes nothing to the simulation.*/ 

	// Create sliding objects
	struct Body *p1, small; p1 = &small;
	struct Body *p2, large; p2 = &large;

	// Set initial conditions
	small.m = 1.0;
	small.x = 1.0-v_init;
	small.v = 0.0;
	large.m = pow(10, 2*(n-1));
	large.x = 1.0;
	large.v = -v_init;

	// Initialise simulation variables
	int collision_type;
	bool finished = false;
	unsigned long int cycles = 0;
	unsigned long int collision_counter = 0;
	double u1;
	
	// Start simulation
	while (!finished) {

		// Increment cycles
		cycles++;

		// Move objects
		small.x += small.v;
		large.x += large.v;

		// Check for collisions
		collision_type = check_collisions(small.x, large.x);

		// Deal with collisions
		switch (collision_type) {

			case 0:
				break;

			case 1:
				small.v = -small.v; // reverse Body's direction
				collision_counter++;
				break;

			case 2:
				p_transfer(p1, p2);
				collision_counter++;
				break;

			default:
				printf("The simulation failed because the temporal resolution is too low to resolve individual collisions.\n");
				break;

		}

		if (cycles%10000 == 0) {
			finished = is_finished(small.v, large.v);
		}

	}

	printf("The simulation counted %ld collisions after %ld increments.\n", collision_counter, cycles);
}

int main() {

	int n = 8;
	double v_init = 0.1;

	run_sim(n, v_init);

	return 0;
}
