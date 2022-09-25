// The following Test Harness implementation has been inspired by Eugene's example(s) given in his Tuesday Section (held at 10/12/21).
// Particularly, ideas such as using an enumeration to iterate over, function pointers, and creating an array of the sorting function names
// have been accomodated in the design/creation of this file.

#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "haeisqn:p:r:"

// Enum used as a the indices of the functions for the function pointers array,
// and also stores names of other options of the test harness (n,p,r,h).
typedef enum { HEAP, SHELL, INSERT, QUICK, NUM_SORTS, LEN, PRINT, SEED, HEADER } Sorts;

// Sorting function names.
const char *names[] = { "Heap Sort", "Shell Sort", "Insertion Sort", "Quick Sort" };

// Bitmasks values to 30-bits.
static const uint32_t bitmask = 0x3fffffff;

// Define array for function pointers.
void (*ptr[4])(Stats *stats, uint32_t *A, uint32_t n);

// Usage: This function is used to initialize the elements of an array to pseudo-random numbers.
//
// Return value: void.
// Parameters: an array, array length, a seed.
void make_array(uint32_t *A, uint32_t n, uint32_t seed) {
    srandom(seed);
    for (uint32_t i = 0; i < n; i += 1) {
        A[i] = random() & bitmask;
    }
    return;
}

int main(int argc, char **argv) {
    int opt = 0;

    // Initialize index-based function pointers.
    ptr[0] = heap_sort;
    ptr[1] = shell_sort;
    ptr[2] = insertion_sort;
    ptr[3] = quick_sort;

    // Define variables for n, p, and r optarg.
    static uint32_t n;
    static uint32_t print_num;
    static uint32_t seed;

    Set s = empty_set();
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': {
            s = insert_set(HEADER, s);
            break;
        }
        case 'a': {
            s = insert_set(HEAP, s);
            s = insert_set(INSERT, s);
            s = insert_set(SHELL, s);
            s = insert_set(QUICK, s);
            break;
        }
        case 'e': {
            s = insert_set(HEAP, s);
            break;
        }
        case 'i': {
            s = insert_set(INSERT, s);
            break;
        }
        case 's': {
            s = insert_set(SHELL, s);
            break;
        }
        case 'q': {
            s = insert_set(QUICK, s);
            break;
        }
        case 'n': {
            n = atoi(optarg);
            s = insert_set(LEN, s);
            break;
        }
        case 'p': {
            print_num = atoi(optarg);
            s = insert_set(PRINT, s);
            break;
        }
        case 'r': {
            seed = atoi(optarg);
            s = insert_set(SEED, s);
            break;
        }
        default: {
            s = insert_set(HEADER, s);
            break;
        }
        }
    }
    if (member_set(HEADER, s) || s == empty_set()) {
        printf("SYNOPSIS\n"
               "    A collection of comparison-based sorting algorithms.\n"
               "\n"
               "USAGE\n"
               "   ./sorting [-haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n"
               "\n"
               "OPTIONS\n"
               "   -h              display program help and usage.\n"
               "   -a              enable all sorts.\n"
               "   -e              enable Heap Sort.\n"
               "   -i              enable Insertion Sort.\n"
               "   -s              enable Shell Sort.\n"
               "   -q              enable Quick Sort.\n"
               "   -n length       specify number of array elements (default: 100).\n"
               "   -p elements     specify number of elements to print (default: 100).\n"
               "   -r seed         specify random seed (default: 13371453).\n");
        return 0;
    }

    // Conditionals to set default values if true.
    if (!member_set(LEN, s)) {
        n = 100;
    }
    if (!member_set(PRINT, s)) {
        print_num = 100;
    }
    if (!member_set(SEED, s)) {
        seed = 13371453;
    }

    // If the number of printed elements exceeds the length of the array, change
    // the number of printed elements to the length of the array.
    if (print_num > n) {
        print_num = n;
    }

    // Stats pointer.
    Stats stat;
    stat.moves = 0;
    stat.compares = 0;

    // Allocate memory.
    uint32_t *array = (uint32_t *) calloc(n, sizeof(uint32_t));

    // HEAP has value 0 in enum.
    for (Sorts x = HEAP; x < NUM_SORTS; x += 1) {

        if (member_set(x, s)) {
            reset(&stat);
            make_array(array, n, seed);
            (*ptr[x])(&stat, array, n);
            printf("%s", names[x]);

            printf(", %" PRIu32 " elements", n);
            printf(", %" PRIu64 " moves", stat.moves);
            printf(", %" PRIu64 " compares\n", stat.compares);

            for (uint32_t i = 0; i < print_num; i += 1) {
                printf("%13" PRIu32, array[i]);
                if ((i + 1) % 5 == 0 || ((i + 1) % 5 != 0 && i == (print_num - 1))) {
                    printf("\n");
                }
            }
        }
        make_array(array, n, seed);
    }
    // Deallocate memory.
    free(array);
    return 0;
}
