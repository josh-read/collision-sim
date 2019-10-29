#!/usr/bin/env python

import numpy as np
from sys import argv

class Body:
    def __init__(self, m, x_init, v_init):
        self.m = m # in kg
        self.x = x_init # in m
        self.v = v_init # in m/s

def p_transfer(u1, u2, m1, m2):

    # from conservation of energy and momentum
    v1 = ((m1-m2)/(m1+m2))*u1 + ((2*m2)/(m1+m2))*u2
    v2 = ((2*m1)/(m1+m2))*u1 + ((m2-m1)/(m1+m2))*u2
    return v1, v2

def check_collisions(x1, x2):

    collision_type = 0 # no collision

    if x1 <= 0: # if collision with wall
        collision_type += 1
    if x2 <= x1: # if collision between boxes
        collision_type += 2

    # collision_type = 3 if both collisions are detected

    return collision_type

def is_finished(v1, v2):
    if v2>v1 and v1>0:
        return True
    else:
        return False

def main(n, v_init):
    """v_init is now used as the measure of precision of the simulation thus reducing arithmetic calculating distance steps.
    The simulation now only runs until no more collisions will occur. This check is somewhat demanding so only runs every 1000 iterations.
    Error handling is more straightforward reducing number of checks per loop.
    Reduced the starting distance between two bodies as this is a waste of time, the distance is now set such that they collide after first interval."""

    # init objects
    small = Body(1, 1.-v_init, 0.) # stationary at 30 cm from wall
    large = Body(10**(2*(n-1)), 1., -v_init) # moving to left at 10 cm/s from 1m away

    # run simulation
    cycles = 0
    finished = False
    collision_counter = 0
    while not finished:

        # increment cycles
        cycles += 1

        # move bodies
        small.x += small.v
        large.x += large.v

        # check for collisions
        collision_type = check_collisions(small.x, large.x)

        # deal with collisions
        if collision_type == 0:
            pass # do nothing
        elif collision_type == 1:
            small.v = - small.v # reverse direction of small body
            collision_counter += 1
        elif collision_type == 2:
            small.v, large.v = p_transfer(small.v, large.v, small.m, large.m) # adjust velocity of both
            collision_counter += 1
        else:
            print("Temporal resolution too low to resolve individual collisions.")
            break

        if cycles%1000 == 0: # finshed check is more demanding so only run after every 1000 increments
            finished = is_finished(small.v, large.v)


    print(f"The simulation counted {collision_counter} collisions after {cycles} increments.")

if __name__ == "__main__":

    try:
        n = int(argv[1])
        v_init = float(argv[2])
    except Exception as e:
        print(e)

    main(n, v_init)
