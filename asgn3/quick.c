// The following functions written use the pseudocode provided in the asgn 3 document by Prof Long.

#include "quick.h"

#include "stats.h"

#include <stdint.h>
#include <stdio.h>

// Usage: This helper function partitions an array and places elements less than the pivot on the left side of the pivot,
// then places values that are greater than the pivot on the right side of the pivot.
//
// Return value: the index of the partition.
// Parameters: stats, an array, a low value, a higher value.
uint32_t partition(Stats *stats, uint32_t *A, uint32_t low, uint32_t high) {
    uint32_t i = low - 1;
    for (uint32_t j = low; j < high; j += 1) {
        if (cmp(stats, A[j - 1], A[high - 1]) < 0) {
            i += 1;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[high - 1]);
    return (i + 1);
}

// Usage: This helper function is used to recursively partition and sort the array over and over again until it is sorted.
//
// Return value: void.
// Parameters: stats, an array, a low value, a high value.
void quick_sorter(Stats *stats, uint32_t *A, uint32_t low, uint32_t high) {
    if (low < high) {
        uint32_t p = partition(stats, A, low, high);
        quick_sorter(stats, A, low, (p - 1));
        quick_sorter(stats, A, (p + 1), high);
    }
    return;
}

// Usage: This function implements the quick sort algorithm to sort an array in increasing order.
//
// Return value: void.
// Parameters: address of a stats pointer, an array, length of array.
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
    return;
}
