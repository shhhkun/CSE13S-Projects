// The write_bin function has been implemented and based off of provided pseudocode by the tutor Jason Yang. This particular implementation
// was chosen to avoid a prior recursive function I implemented which may have caused issues regarding recursion limits.
#include "node.h"
#include "pq.h"
#include "io.h"
#include "code.h"
#include "huffman.h"
#include "defines.h"
#include "header.h"
#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <stdlib.h>

#define OPTIONS       "hi:o:v"
#define STDIN_FILENO  0
#define STDOUT_FILENO 1

static bool header = false;
static bool stdouts = true;
static bool stdins = true;
static bool verbose = false;

static uint64_t decoded_symbols = 0; // used in write_bin() function

// Usage: This function will read the header of the infile into a buffer that points to the header struct.
//        The contents of the header (now in the struct) is then used to determine information regarding
//        the infile to be decoded.
//
// Parameters: infile and the address of the header struct where the read bytes will be stored.
// Return value: void.
void load_header(int infile, Header *h) {
    Header *head = h;
    uint8_t *hbuf = (uint8_t *) head;
    read_bytes(infile, hbuf, sizeof(Header));
    return;
}

// Usage: This function will read the emitted binary of infile and iterate over it bit by bit. For each bit that is read
//        if it is 0, then we traverse to the left of the tree. Otherwise the bit read is 1, and we traverse right. Whenever
//        a leaf node of the Huffman tree is reached the corresponding symbol of the node will be buffered to the buffer.
//        This process will be repeated until the number of decoded symbols is equal to the original file size.
//
// Parameters: infile, the file size, a buffer, and the root of the Huffman tree.
// Return value: void.
void write_bin(int infile, uint64_t file_size, uint8_t *binary_buf, Node *root) {
    Node *curr = root;
    while (decoded_symbols != file_size) {
        uint8_t bit;
        read_bit(infile, &bit);
        if (bit == 0) {
            curr = curr->left;
        } else { // else bit is 1
            curr = curr->right;
        }
        if (curr->left == NULL && curr->right == NULL) { // if at leaf node
            binary_buf[decoded_symbols] = curr->symbol; // buffer the symbol
            curr = root; // reset to start root of tree
            decoded_symbols += 1;
        }
    }
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
            break;
        }
        case 'o': {
            stdouts = false;
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC);
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
                        "  A Huffman decoder.\n"
                        "  Decompresses a file using the Huffman coding algorithm.\n"
                        "\n"
                        "USAGE\n"
                        "  ./decode [-h] [-i infile] [-o outfile]\n"
                        "\n"
                        "OPTIONS\n"
                        "  -h             Program usage and help.\n"
                        "  -v             Print compression statistics.\n"
                        "  -i infile      Input file to decompress.\n"
                        "  -o outfile     Output of decompressed data.\n");
        return 0;
    }

    if (stdins) {
        infile = STDIN_FILENO;
    }
    if (stdouts) {
        outfile = STDOUT_FILENO;
    }

    Header rhead;
    load_header(infile, &rhead);

    if (rhead.magic != MAGIC) {
        fprintf(stderr, "Invalid magic number.\n");
        exit(1);
    }

    // set perms to outfile as specified in the header of infile
    fchmod(outfile, rhead.permissions);

    // read tree_dump into dumped_tree buffer
    uint8_t dumped_tree[MAX_TREE_SIZE];

    read_bytes(infile, dumped_tree, rhead.tree_size);

    // get root node
    Node *root = rebuild_tree(rhead.tree_size, dumped_tree);

    uint8_t binary_buf[rhead.file_size];

    write_bin(infile, rhead.file_size, binary_buf, root);
    write_bytes(outfile, binary_buf, rhead.file_size); // write out decoded message.

    if (verbose) {
        double compressed = bytes_read;
        double decompressed = bytes_written;
        double space_saving = 100.0 * (1.0 - (compressed / decompressed));
        fprintf(stderr,
            "Compressed file size: %.0lf bytes\n"
            "Decompressed file size: %.0lf bytes\n"
            "Space saving: %.2lf%%\n",
            compressed, decompressed, space_saving);
    }

    delete_tree(&root);
    close(infile);
    close(outfile);
    return 0;
}
