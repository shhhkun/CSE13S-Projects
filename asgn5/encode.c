#include "node.h"
#include "pq.h"
#include "io.h"
#include "code.h"
#include "huffman.h"
#include "defines.h"
#include "header.h"

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/stat.h>

#define OPTIONS       "hi:o:v"
#define STDIN_FILENO  0
#define STDOUT_FILENO 1

static bool header = false;
static uint16_t symbols = 0;
static bool stdouts = true;
static bool stdins = true;
static bool verbose = false;

struct stat fbuf;

// Usage: This function will write the contents of the constructed header struct to outfile.
//
// Parameters: outfile and the address of the header struct to be written.
// Return value: void.
void dump_header(int outfile, Header *h) {
    Header *head = h;
    uint8_t *hbuf = (uint8_t *) head;
    write_bytes(outfile, hbuf, sizeof(Header));
    return;
}

int main(int argc, char **argv) {
    int infile;
    int outfile;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': {
            header = true;
            break;
        }
        case 'i': {
            stdins = false;
            infile = open(optarg, O_RDONLY);
            fstat(infile, &fbuf);
            break;
        }
        case 'o': {
            stdouts = false;
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC);
            fchmod(outfile, fbuf.st_mode);
            break;
        }
        case 'v': {
            verbose = true;
            break;
        }
        default: {
            header = true;
            break;
        }
        }
    }
    if (header) {
        fprintf(stderr, "SYNOPSIS\n"
                        "  A Huffman encoder.\n"
                        "  Compresses a file using the Huffman coding algorithm.\n"
                        "\n"
                        "USAGE\n"
                        "  ./encode [-h] [-i infile] [-o outfile]\n"
                        "\n"
                        "OPTIONS\n"
                        "  -h             Program usage and help.\n"
                        "  -v             Print compression statistics.\n"
                        "  -i infile      Input file to compress.\n"
                        "  -o outfile     Output of compressed data.\n");
        return 0;
    }
    // if not specified, set infile or outfile to stdin/stdout.
    if (stdins) {
        infile = STDIN_FILENO;
    }
    if (stdouts) {
        outfile = STDOUT_FILENO;
    }

    uint8_t buf[BLOCK];

    // Histogram
    uint64_t hist[ALPHABET];
    // zero it out
    for (uint32_t i = 0; i < ALPHABET; i += 1) {
        hist[i] = 0;
    }
    hist[0] += 1;
    hist[255] += 1;

    // populate histogram
    int bytes;
    do {
        bytes = read_bytes(infile, buf, BLOCK);
        for (int i = 0; i < bytes; i += 1) {
            hist[buf[i]] += 1;
        }
    } while (bytes > 0);

    Node *root = build_tree(hist);

    // create code table
    Code table[ALPHABET];

    build_codes(root, table);

    // count number of unique symbols
    for (uint32_t i = 0; i < ALPHABET; i += 1) {
        if (hist[i] > 0) {
            symbols += 1; // count number of unique symbols
        }
    }

    // construct header
    Header head = { .magic = MAGIC,
        .permissions = fbuf.st_mode,
        .tree_size = (3 * symbols) - 1,
        .file_size = fbuf.st_size };
    dump_header(outfile, &head);

    // dump tree contents to outfile
    dump_tree(outfile, root);

    // reset to read from top of infile
    lseek(infile, 0, SEEK_SET);
    int count;
    do {
        count = read_bytes(infile, buf, BLOCK);
        for (int i = 0; i < count; i += 1) {
            write_code(outfile, &table[buf[i]]);
        }
    } while (count > 0);
    flush_codes(outfile);

    if (verbose) {
        double uncompressed = bytes_read / 2.0; // account for the second read of infile.
        double compressed = bytes_written;
        double space_saving = 100.0 * (1.0 - (compressed / uncompressed));
        fprintf(stderr,
            "Uncompressed file size: %.0lf bytes\n"
            "Compressed file size: %.0lf bytes\n"
            "Space saving: %.2lf%%\n",
            uncompressed, compressed, space_saving);
    }

    delete_tree(&root);
    close(infile);
    close(outfile);
    return 0;
}
