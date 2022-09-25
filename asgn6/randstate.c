#include <stdint.h>
#include <gmp.h>

#include "randstate.h"

gmp_randstate_t state; // extern variable

// Usage: This function will initialize the (extern/global) state.
//
// Parameters: a seed.
// Return value: void.
void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
    return;
}

// Usage: This function will clear the (extern/global) state.
//
// Parameters: void.
// Return value: void.
void randstate_clear(void) {
    gmp_randclear(state);
    return;
}
