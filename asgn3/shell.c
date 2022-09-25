// The following functions written use the pseudocode provided in the asgn 3 document by Prof Long.

#include "shell.h"

#include "stats.h"

#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t gap;
static uint32_t gap_index;

// Usage: This helper function mimicks a generator and produces gaps to be used in the shell_sort function.
// The gaps returned will be different every time, and will stop when the gap reaches a numerical value of 1.
//
// Return value: a gap value.
// Parameters: void.
uint32_t gaps(void) {
    gap = ((pow(3, gap_index) - 1) / 2);
    gap_index -= 1;
    return gap;
}

// Usage: this function implements the shell sort algorithm to sort an array in increasing order.
//
// Return value: void.
// Parameters: address of a stats pointer, an array, length of array.
void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    gap_index = (log(3 + 2 * n) / log(3));
    do {
        gaps();
        for (uint32_t i = gap; i < n; i += 1) {
            uint32_t j = i;
            uint32_t temp = move(stats, A[i]);
            while (j >= gap && cmp(stats, temp, A[j - gap]) < 0) {
                A[j] = move(stats, A[j - gap]);
                j -= gap;
            }
            A[j] = move(stats, temp);
        }
    } while (gap > 1);
    return;
}
