#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

def circle_formula(x, r):
	y = np.sqrt(r**2 - x**2) # rearranged from x^2 + y^2 = r^2
	return y

def vect_dist(v1, v2):
	dx = v1[0] - v2[0] # change in x
	dy = v1[1] - v2[1] # change in y
	dist = np.sqrt(np.power(dx,2) + np.power(dy, 2)) # pythagoras
	return dist

def find_pi(steps):

	r = 1 # Radius shouldn't matter so long as it is consistent
	x = np.linspace(0, r, steps) # x ranging from origin to radius
	y = circle_formula(x, r) # calculate corresponding y values

	vect_list = np.vstack((x,y)).T # stack x and y values to make next step easier

	# Now going to add up each segment to get the length of the arc
	curve_len = 0. # initialise curve length
	for i in range(len(vect_list)-1):
		curve_len += vect_dist(vect_list[i], vect_list[i+1]) # add each segment of the curve

	c = 4*curve_len # we only measured 1/4 of a circle
	d = 2*r # diameter is two times radius

	pi = c/d # from definition pi*d = circumfrence

	return pi

if __name__ == "__main__":

	steps = [10, 100, 1000, 10000, 100000, 1000000] # number of circle points in calculation
	pis = [] # empty list to house calculated values of pi

	for step in steps:
		pis.append(find_pi(step)) # find pi using varying numbers of steps

	# Print difference between my value and numpy value of pi
	print(f"There is a difference of {np.abs(np.pi-pis[-1])} at {steps[-1]} steps.")

	# Plot values
	plt.plot([min(steps), max(steps)], [np.pi, np.pi]) # plot actual value of pi
	plt.semilogx(steps, pis) # plot my values of pi
	plt.show() # show graph
	# Whole script runs in 12.4s on my PC, if taking too lower max nr of steps
