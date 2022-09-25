#include "mathlib.h"

#include <stdio.h>

static int count = 0;

// Usage: pi_euler is a function used to compute the mathematical constant pi.
// It uses Euler's series to iteratively sum the value of pi.
//
// Return value: the computed constant pi.
// Parameters: none.
double pi_euler(void) {
    double total = 0.0, current = 1.0, exponent, result;
    for (double k = 1.0; current > EPSILON; k += 1.0) {
        count += 1;
        exponent = k * k;
        current = 1.0 / exponent;
        total += current;
    }
    total = 6.0 * total;
    result = sqrt_newton(total);
    return result;
}

// Usage: used to keep track of the number of iterations of the above function, pi_euler.
//
// Return value: the amount of iterations needed to compute pi.
// Parameters: none.
int pi_euler_terms(void) {
    return count;
}
