#include "mathlib.h"

#include <stdio.h>

static int count = 0;

// Usage: e is a function used to compute the mathematical constant e.
// It uses a Euler's formula to iteratively compute the constant e.
//
// Return value: the computed constant e.
// Parameters: none.
double e(void) {
    double total = 0.0, prev = 1.0, x_over_k, current = 1.0;
    for (double k = 0.0; current > EPSILON; k += 1.0) {
        count += 1;
        if (k == 0.0) { // if on the first iteration, then the first term to be added is 1
            total += prev;
        } else {
            x_over_k = 1.0 / k;
            current = prev * x_over_k;
            total += current;
            prev = current;
        }
    }
    return total;
}

// Usage: used to keep track of the number of iterations of the above function, e.
//
// Return value: the amount of iterations needed to compute e.
// Parameters: none.
int e_terms(void) {
    return count;
}
