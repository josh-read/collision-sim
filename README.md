# Collisions Simulator

## Background

This simulation is inspired by a video by youtuber 3Blue1Brown: https://www.youtube.com/watch?v=HEfHFsfGXjs.
The idea is that the simulation computes pi by recording the number of collisions between two objects with known mass and a wall.

For an explanation on why this is true, his follow up video explains it very well: https://www.youtube.com/watch?v=jsYwFizhncE.

## Code

I started writing this code when I first started learning C, as I thought it was a project that would really benefit from C's speed.
My first attempt `C-implementation/collision-sim.c` only worked up to 5 digits (4 d.p.) of pi.
At 6 digits and above, I encountered a bug whereby the program to reported it wasn't running at high enough resolution (two collisions detected in one time step), yet the collision counter was way higher than should have been possible.
I spent ages trying to debug this issue and ultimately failed to diagnose and fix the issue.
As I was working on the C version I realised there were several places that could be greatly optimised, so I decided to rewrite a working model in Python and then translate it across to C.

The first main area I improved was the way the system was incremented.
Originially I had been using fractions of seconds as a timestep to increment the system and ultimately control resolution.
I realised it would be simpler to control the resolution with the initial velocity, instead incrementing the system 1 second at a time.
This has a couple of speed benefits for the main loop:
1. Time can now be stored as an integer which is quicker to increment an integer than add two floats.
2. Position adjustment requires one addition operation (x = x+v) rather than multiplication and addition (x = x+(v*dt))

Another big improvement was changing the way the loop worked to automatically check if it was finished rather than running for a set amount of time.
This extra check takes a fair bit of time so it only checks every 1000 seconds, resulting in a 15% decrease in runtime for 7 digits of pi.

Finally I hardcoded the starting positions to depend on the initial velocity so that no time was wasted moving the bodies while they are not close to each other.

This optimised version ran without issue and it was fairly simple to rewrite it in C, experimenting with pointers and other tricks along the way.
The times below were for the best optimised versions of Python and C code I could write at the time and were measured with GNU time on an i7-2600K.

| Digits of Pi | Python (s) | C (s) |
|:------------:|:----------:|:-----:|
|       7      |     3.6    |  0.06 |
|       8      |    34.4    |  0.5  |
|       9      |            |  5.3  |

This speed difference shows the huge performance increase avaliable by switching to C, and I filed this project away happy that learning C had given me the speed boost I'd hoped for...

...Until I returned to this project today.
I wanted to try out the Cython library after watching a couple of talks on it.
I'd been aware of it for some time but having seen speedups up to 200x in examples I wanted to give it a go.
Really all I did to my code was add typing information, use the native C math function (pow) where nescessary, and compile.
Since last working on this project I've learned about the gcc -O flag to optimise C code so I retimed my code using -O3 compiled code.


| Digits of Pi | C (s) | Cython (s) |
|:------------:|:-----:|:----------:|
|       8      |  0.4  |     0.4    |
|       9      |  4.2  |     4.2    |

Its obvious from the table that I've managed to make my python code as fast as optimised C.
This astounded me and I'm really glad I've learned this is possible as it was so much quicker writing the Python code which I am more used to.
