// The design of read_bytes, write_bytes, read_bit, flush_codes have been inspired and based off of Eugene Chou's explanations of
// the functions in his 11/2/21 section.
#include "code.h"
#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

// static variables for read_bit
static uint8_t byte_buf[BLOCK];
static uint32_t bit_index = 0;
static uint32_t end = 0;

// static variables for write_code
static uint8_t code_buf[BLOCK];
static uint32_t code_index = 0;

// extern variables for stats
uint64_t bytes_read;
uint64_t bytes_written;

// Usage: This function uses syscall "read" to read bytes from infile in chunks of 4096 bytes, which is referred to as
//        "BLOCK" as specified in the defines.h header, which lists macros. Though if read fails to read all the bytes,
//        the function will continue to loop calls to read.
//
// Parameters: an infile, a buffer, and nbytes to read.
// Return value: the amount of bytes read from infile.
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int count;
    int bytes_r = 0;
    do {
        count = read(infile, buf + bytes_r, nbytes - bytes_r);
        bytes_r += count;
        bytes_read += count; // extern variable for stats
    } while (count > 0);
    return bytes_r;
}

// Usage: This function uses syscall "write" to write bytes to an outfile, the specified amount of bytes written to
//        outfile is specified with "nbytes". Though if write fails to write all the bytes specified by nbytes to the
//        outfile, then it will continue to loop cals to write.
//
// Parameters: an outfile, a buffer, and nbytes to write.
// Return value: the amount of bytes written to outfile.
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int count;
    int bytes_w = 0;
    do {
        count = write(outfile, buf + bytes_w, nbytes - bytes_w);
        bytes_w += count;
        bytes_written += count; // extern variable for stats
    } while (count > 0);
    return bytes_w;
}

// Usage: This function will read a bit from an infile one at a time, so the first call to this function will read the first
//        bit of the first byte from infile. This function keeps track of which byte and bit to read, and thus if called again
//        it will read the next bit of the byte if the byte has not ended, or if it has it will move onto the next byte.
//
//        read_bit() will continue to return true if not all bits from infile have been read, and will return false if all bits
//        have been read from infile.
//
// Parameters: an infile and an address of a bit variable to store the bit value read when read_bit() is called.
// Return value: a boolean to indicate whether all bits have been read from infile.
bool read_bit(int infile, uint8_t *bit) {
    if (bit_index == 0) { // if bit_index is 0 read in a new BLOCK of bytes.
        int read = read_bytes(infile, byte_buf, BLOCK);
        if (read < BLOCK) { // if less than a BLOCK is read, set the end byte.
            end = read * 8 + 1;
        }
    }
    *bit = (byte_buf[bit_index / 8] >> bit_index % 8) & 0x1;
    bit_index += 1;
    if (bit_index / 8 == BLOCK) { // if 4096 * 8 bits were read, reset bit_index
        bit_index = 0;
    }
    return bit_index != end;
}

// Usage: This function will check a bit from a code and copy that bit over to a buffer. Static variables are used to keep track of
//        which byte, and the bit of that byte are to be currently copied over to the buffer. Once all the bits in the code have been
//        copied over to the buffer, then we write the contents of the buffer to the outfile. The amount of bits (bytes) written to
//        outfile are determined with the amount of bits (or bytes) in the code.
//
// Parameters: an outfile and a code.
// Return value: void.
void write_code(int outfile, Code *c) {
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        if (code_get_bit(c, i) == true) {
            code_buf[code_index / 8] |= 0x1 << code_index % 8; // set bit to 1
        } else {
            code_buf[code_index / 8] &= ~(0x1 << code_index % 8); // clr bit to 0
        }
        code_index += 1;

        if (code_index / 8 == BLOCK) {
            write_bytes(outfile, code_buf, BLOCK);
            code_index = 0;
        }
    }
    return;
}

// Usage: This function is for the case in which the code written to the buffer "code_buf" is not a full BLOCK of bits, which is 4096
//        bytes or 32768 bits. If so, write_code will not write out to outfile, so instead flush_codes will write out the contents of
//        "code_buf" instead.
//        Before flushing the codes we check if the last bit in the buffer completes a byte. If not, we zero out the bits after the
//        last bit in code put in the buffer until it fulfills the constraints of a full byte.
//
// Parameters: an outfile.
// Return value: void.
void flush_codes(int outfile) {
    if (code_index > 0) { // write_code's buffer was not full
        while (code_index % 8 != 0) { // if not at the start of a new byte
            code_buf[code_index / 8] &= ~(0x1 << code_index % 8);
            code_index += 1;
        }
        write_bytes(outfile, code_buf, code_index / 8);
    }
    return;
}
