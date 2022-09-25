#include "mathlib.h"

#include <stdio.h>

static int count = 0;

// Usage: pi_madhava is a function used to compute the mathematical constant pi.
// It uses The Madhava Series to iteratively compute the constant pi.
//
// Return value: the computed constant pi.
// Parameters: none.
double pi_madhava(void) {
    double current = 1.0, exponent = 1.0, exp_fraction, bottom, total = 0.0;
    for (double k = 0.0; absolute(current) > EPSILON; k += 1.0) {
        count += 1;
        exp_fraction = 1.0 / exponent;
        bottom = (2.0 * k) + 1.0;
        current = exp_fraction / bottom;
        total += current;
        exponent *= -3.0;
    }
    total = sqrt_newton(12.0) * total;
    return total;
}

// Usage: used to keep track of the number of iterations of the above function, pi_madhava.
//
// Return value: the amount of iterations needed to compute pi.
// Parameters: none.
int pi_madhava_terms(void) {
    return count;
}
