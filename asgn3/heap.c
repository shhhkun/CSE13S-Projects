// The following functions written use the pseudocode provided in the asgn 3 document by Prof Long.

#include "heap.h"

#include "stats.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Usage: This helper function identifies a max child that will be used as a pivot to identify the position of the right and left child.
//
// Return value(s): right or left child.
// Parameters: stats, an array, a first value, a last value.
uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && cmp(stats, A[right - 1], A[left - 1]) > 0) {
        return right;
    }
    return left;
}

// Usage: This helper function fixes the heap that build_heap has made, in such a way that the largest element can be taken again.
//
// Return value: void.
// Parameters: stats, an array, a first value, a last value.
void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);

    while (mother <= floor(last / 2) && !found) {
        if (cmp(stats, A[mother - 1], A[great - 1]) < 0) {
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = true;
        }
    }
    return;
}

// Usage: this helper function takes an array and creates a max heap, meaning that the largest element becomes the first element.
//
// Return value: void.
// Parameters: stats, an array, a first value, a last value.
void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t father = floor(last / 2); father > first - 1; father -= 1) {
        fix_heap(stats, A, father, last);
    }
    return;
}

// Usage: This function implements the heap sort algorithm to sort an array in increasing order.
//
// Return value: void.
// Parameters: address of a stats pointer, an array, length of array.
void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(stats, A, first, last);
    for (uint32_t leaf = last; leaf > first; leaf -= 1) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, first, leaf - 1);
    }
    return;
}
