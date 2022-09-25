#include "bv.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Usage: Helper function to calculate the ceiling of two numbers x and y.
//
// Parameters: numerator x, denominator y.
// Return value: ceiling of x and y.
uint32_t ceiling(uint32_t x, uint32_t y) {
    return (x + y - 1) / y;
}

// Usage: Constructor of a bit vector. The vector will be in bytes, but will be
//        created based on the length (in bits) as specified.
//
// Parameters: length of bit vector (in bits).
// Return value: bit vector.
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) calloc(ceiling(bv->length, 8), sizeof(uint8_t));
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

// Usage: Destructor of a bit vector.
//
// Parameters: bit vector.
// Return value: void.
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
}

// Usage: This function returns the length of a bit vector (in bits).
//
// Parameters: bit vector.
// Return value: length of the vector in bits.
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// Usage: This function will set the i'th bit of a bit vector to 1. Where i
//        is the index of the bit to set.
//
// Parameters: bit vector and an index.
// Return value: boolean to signify whether the bit was set 1 or not.
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i < 0 || i >= bv->length) {
        return false;
    }
    bv->vector[i / 8] |= 0x1 << i % 8;
    return true;
}

// Usage: This function will clear the i'th bit of a bit vector to 0. Where
//        i is the index of the bit to clear.
//
// Parameters: bit vector and an index.
// Return value: boolean to signify whether the bit was cleared to 0 or not.
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i < 0 || i >= bv->length) {
        return false;
    }
    bv->vector[i / 8] &= ~(0x1 << i % 8);
    return true;
}

// Usage: This function will retrieve the i'th bit of a bit vector. And will
//        return true/false based on whether that bit is 1.
//
// Parameters: bit vector and an index.
// Return value: true if the bit is 1, false otherwise.
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (((bv->vector[i / 8] >> i % 8) & 0x1) == 1) {
        return true; // bit is 1
    }
    return false; // bit is 0
}

// Usage: Debugger function for a bit vector, that will print the contents of
//        a bit vector (0's and 1's).
//
// Parameters: bit vector.
// Return value: void.
void bv_print(BitVector *bv) {
    printf("vector: ");
    for (uint32_t i = 0; i < bv->length; i += 1) {
        if (bv_get_bit(bv, i) == true) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}
