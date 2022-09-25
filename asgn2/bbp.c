#include "mathlib.h"

#include <stdio.h>

static int count = 0;

// Usage: pi_bbp is a function used to compute the mathematical constant pi.
// It uses The Bailey-Borwein-Plouffe-Formula to iteratively compute the constant pi.
//
// Return value: the computed constant pi.
// Parameters: none.
double pi_bbp(void) {
    double total = 0.0, current = 1.0, fraction1 = 1.0, fraction2, exponent = 1.0, top, bottom;
    for (double k = 0.0; current > EPSILON; k += 1.0) {
        count += 1;
        fraction1 = 1.0 / exponent;

        top = (k * ((120.0 * k) + 151.0) + 47.0);
        bottom = (k * (k * (k * ((512.0 * k) + 1024.0) + 712.0) + 194.0) + 15.0);
        fraction2 = top / bottom;

        current = fraction1 * fraction2;
        total += current;
        exponent *= 16.0;
    }
    return total;
}

// Usage: used to keep track of the number of iterations of the above function, pi_bbp.
//
// Return value: the amount of iterations needed to compute pi.
// Parameters: none.
int pi_bbp_terms(void) {
    return count;
}
