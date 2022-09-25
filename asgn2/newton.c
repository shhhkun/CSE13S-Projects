#include "mathlib.h"

#include <stdio.h>

static int count = 0;

// Usage: sqrt_newton is a function used to compute the square root of a number.
// It uses a formula derived from newton-raphsons method, the design used to write the code below
// is provided in the asgn2 document by Prof. Long.
//
// Return value: the computed square root of a given type "double" number.
// Parameters: a single "double" type number.
double sqrt_newton(double x) {
    double lower = 0.0, upper = 1.0;
    count = 0;
    while (absolute(upper - lower) > EPSILON) {
        count += 1;
        lower = upper;
        upper = 0.5 * (lower + (x / lower));
    }
    return upper;
}

// Usage: used to keep track of the number of iterations of the above function, sqrt_newton.
//
// Return value: the total number of iterations it took to compute a square root of a number.
// Parameters: none.
int sqrt_newton_iters(void) {
    return count;
}
