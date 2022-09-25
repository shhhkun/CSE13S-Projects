// The following functions written use the pseudocode provided in the asgn 3 document by Prof Long.

#include "insert.h"

#include "stats.h"

#include <stdint.h>
#include <stdio.h>

// Usage: This function implements the insertion sort algorithm to sort an array in increasing order.
//
// Return value: void.
// Parameters: address of a stats pointer, an array, length of array.
void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    for (uint32_t k = 1; k < n; k += 1) {
        uint32_t j = k;
        uint32_t temp = move(stats, A[k]);
        while (j > 0 && cmp(stats, A[j - 1], temp) > 0) {
            A[j] = move(stats, A[j - 1]);
            j -= 1;
        }
        A[j] = move(stats, temp);
    }
    return;
}
