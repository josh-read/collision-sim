from libc.math cimport pow


cdef (double, double) p_transfer(double u1, double u2, double m1, double m2):
    """Calculates the velcoity of masses m1 and m2."""

    cdef double v1, v2

    # from conservation of energy and momentum
    v1 = ((m1-m2)/(m1+m2))*u1 + ((2*m2)/(m1+m2))*u2
    v2 = ((2*m1)/(m1+m2))*u1 + ((m2-m1)/(m1+m2))*u2

    return v1, v2


cdef int check_collisions(double x1, double x2):
    """Checks for collisions between bodies and wall.

    Returns 0 if there is no collision.
    Returns 1 if there is a collision with the wall.
    Returns 2 if there is a collision between the bodies.
    Returns 3 if there are collisions with wall and bodies."""
    
    cpdef int collision_type 

    collision_type = 0 # no collision

    if x1 <= 0: # if collision with wall
        collision_type += 1
    if x2 <= x1: # if collision between boxes
        collision_type += 2

    return collision_type


cdef bint is_finished(double v1, double v2):
    """Checks if another collision can occur.

    If both bodies are moving away from the wall and the outer body is
    travelling faster, no more collisions are possible."""
    
    if v2>v1 and v1>0.0:
        return True
    else:
        return False


cpdef (size_t, size_t) main(n, v_init):
    """Convert variables to C datatypes and pass to main loop."""

    # Define variables as C datatypes
    cdef double m_small, x_small, v_small
    cdef double m_large, x_large, v_large
    cdef size_t col_count, tot_count

    # Properties of smaller body
    m_small = 1.
    x_small = 1. - v_init
    v_small = 0

    # Properties of larger body
    m_large = pow(10, (2*(n-1)))
    x_large = 1.
    v_large = - v_init

    # Perform main loop
    return main_loop(m_small, x_small, v_small,
                     m_large, x_large, v_large)

cdef (size_t, size_t) main_loop(double m_small, double x_small,
                                double v_small, double m_large,
                                double x_large, double v_large):
    """Numerical simulation of body dynamics.

    Incrementally move bodies and check for collisions. Upon
    collisions; add to collision counter, adjust velocity
    accordingly and continue moving bodies incrementally."""

    # Define simulation variables as C datatypes
    cdef size_t cycles = 0
    cdef size_t collision_counter = 0
    cdef int collision_type
    cdef bint finished = False

    # Begin main loop
    while not finished:

        # increment cycles
        cycles += 1

        # move bodies
        x_small += v_small
        x_large += v_large

        # check for collisions
        collision_type = check_collisions(x_small, x_large)

        # deal with collisions
        if collision_type == 0:
            pass # do nothing
        elif collision_type == 1:
            v_small = - v_small # reverse direction of small body
            collision_counter += 1
        elif collision_type == 2:
            v_small, v_large = p_transfer(v_small, v_large, m_small, m_large) # adjust velocity of both
            collision_counter += 1
        else:
            print("Temporal resolution too low to resolve individual collisions.")
            break
        
        # check if simulation is finished
        if cycles%1000 == 0:
            finished = is_finished(v_small, v_large)

    return collision_counter, cycles
