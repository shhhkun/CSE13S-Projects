#include "bf.h"
#include "bv.h"
#include "salts.h"
#include "speck.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt
    uint64_t secondary[2]; // Secondary hash function salt
    uint64_t tertiary[2]; // Tertiary hash function salt
    BitVector *filter;
};

// Usage: The constructor for a bloom filter. The primary, secondary, and tertiary salts
//        are set to their respective 64-bit defined macros in salt.h.
//
// Parameters: size of the bloom filter (in bits).
// Return value: bloom filter.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

// Usage: Destructor for a bloom filter.
//
// Parameters: address of bloom filter.
// Return value: void.
void bf_delete(BloomFilter **bf) {
    if (*bf && (*bf)->filter) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
}

// Usage: This function returns the size (length) of a bloom filter (in bits).
//
// Parameters: bloom filter.
// Return value: size of bloom filter (in bits).
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// Usage: This function will insert oldspeak into the bloom filter by setting the 3
//        corresponding bits whose position is calculated by hashing a salt with
//        oldspeak.
//
// Parameters: bloom filter and oldspeak.
// Return value: void.
void bf_insert(BloomFilter *bf, char *oldspeak) {
    bv_set_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf));
}

// Usage: This function will check the bits of a bloom filter and return true if
//        the 3 bits (are most likely) set to 1. Otherwise not all bits were set.
//
// Parameters: bloom filter and oldspeak.
// Return value: boolean indicating whether all bits were set.
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    bool first = bv_get_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf));
    bool second = bv_get_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf));
    bool third = bv_get_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf));
    return first && second && third;
}

// Usage: This function will retrieve the amount of set bits (bit is 1) of the
//        bloom filter.
//
// Parameters: bloom filter.
// Return value: amount of set bits (bit is 1) in the filter.
uint32_t bf_count(BloomFilter *bf) {
    uint32_t set_bits = 0;
    for (uint32_t i = 0; i < bf_size(bf); i += 1) {
        if (bv_get_bit(bf->filter, i) == true) {
            set_bits += 1;
        }
    }
    return set_bits;
}

// Usage: A debug function to print the contents of a bloom filter.
//
// Parameters: bloom filter.
// Return value: void.
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
