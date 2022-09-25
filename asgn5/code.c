#include "code.h"
#include "defines.h"

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

// Usage: This function will be the constructor of the code array of 256 bits long.
//
// Parameters: void.
// Return value: a code.
Code code_init(void) {
    Code code;
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i += 1) {
        code.bits[i] = 0;
    }
    code.top = 0;
    return code;
}

// Usage: This function will return the current size of the code array (in bits).
//
// Parameters: a code.
// Return value: current size of the code.
uint32_t code_size(Code *c) {
    return c->top;
}

// Usage: This function will check whether the code is empty.
//
// Parameters: a code.
// Return value: a boolean indicating whether the code is empty.
bool code_empty(Code *c) {
    if (c->top == 0) {
        return true;
    }
    return false;
}

// Usage: This function will check whether the code is full.
//
// Parameters: a code.
// Return value: a boolean indicating whether the code is full.
bool code_full(Code *c) {
    if (c->top == MAX_CODE_SIZE * 8) {
        return true;
    }
    return false;
}

// Usage: This function serves as a helper function, which will set the bit at index 'i' in the code to 1.
//
// Parameters: a code and the index of the bit.
// Return value: a boolean value indicating success or failure.
bool code_set_bit(Code *c, uint32_t i) {
    if (i < 0 || i >= MAX_CODE_SIZE * 8) {
        return false;
    }
    c->bits[i / 8] |= 0x1 << i % 8;
    return true;
}

// Usage: This function serves as a helper function, which will clear the bit at index 'i' in the code to 0.
//
// Parameters: a code and the index of the bit.
// Return value: a boolean value indicating success or failure.
bool code_clr_bit(Code *c, uint32_t i) {
    if (i < 0 || i >= MAX_CODE_SIZE * 8) {
        return false;
    }
    c->bits[i / 8] &= ~(0x1 << i % 8);
    return true;
}

// Usage: This function serves as a helper function, which will retrieve the value (0 or 1) of the bit at index 'i'
//        of the code.
//
// Parameters: a code and the index of the bit to retrieve.
// Return value: a boolean value indicating success or failure.
bool code_get_bit(Code *c, uint32_t i) {
    if (((c->bits[i / 8] >> i % 8) & 0x1) == 1) { // if bit at index i is 1
        return true;
    }
    return false;
}

// Usage: This function will push a bit (0 or 1) onto the code.
//
// Parameters: a code and the bit to be pushed onto the code.
// Return value: a boolean value indicating success or failure.
bool code_push_bit(Code *c, uint8_t bit) { // assuming that bit is only 0 or 1
    if (code_full(c)) {
        return false;
    }
    if (bit == 1) {
        code_set_bit(c, c->top); // if the bit is 1, we set the bit
    } else {
        code_clr_bit(c, c->top); // else the bit is 0
    }
    c->top += 1;
    return true;
}

// Usage: This function will pop a bit (0 or 1) off the code, and pass that popped bit back to the parameter "bit".
//
// Parameters: a code and an address of a bit where the popped bit will be stored.
// Return value: a boolean value indicating success or failure.
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }
    c->top -= 1;
    if (code_get_bit(c, c->top) == true) {
        *bit = 1; // if true the bit is 1
    } else {
        *bit = 0; // else it means the bit is 0
    }
    return true;
}

// Usage: This function will simply print out the contents of the code, starting from index 0 to code_size.
//
// Parameters: a code.
// Return value: void.
void code_print(Code *c) {
    printf("Code: ");
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        if (code_get_bit(c, i) == true) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
    return;
}
