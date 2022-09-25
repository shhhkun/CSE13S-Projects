#include "mathlib.h"

#include <stdio.h>

static int count = 0;

// Usage: pi_viete is a function used to compute the mathematical constant pi.
// It uses Viete's Formula to iteratively compute the constant pi.
//
// Return value: the computed constant pi.
// Parameters: none.
double pi_viete(void) {
    double total = 1.0, sqrt_current = 1.0, sqrt_previous = 0.0, diff = 1.0;
    while (diff > EPSILON) {
        count += 1;
        sqrt_current = sqrt_newton((2.0 + sqrt_previous));
        total *= (sqrt_current / 2.0);
        diff = absolute(sqrt_current - sqrt_previous);
        sqrt_previous = sqrt_current;
    }
    total = 2.0 / total;
    return total;
}

// Usage: used to keep track of the number of iterations of the above function, pi_viete.
//
// Return value: the amount of iterations needed to compute pi.
// Parameters: none.
int pi_viete_factors(void) {
    return count;
}
