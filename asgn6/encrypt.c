#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define OPTIONS "hvi:o:n:"

static bool header = false;
static bool verbose = false;
static char *pbfile_name = "rsa.pub";

int main(int argc, char **argv) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pbfile;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': {
            header = true;
            break;
        }
        case 'v': {
            verbose = true;
            break;
        }
        case 'i': {
            infile = fopen(optarg, "r");
            break;
        }
        case 'o': {
            outfile = fopen(optarg, "w");
            break;
        }
        case 'n': {
            pbfile_name = optarg;
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
                        "   Encrypts data using RSA encryption.\n"
                        "   Encrypted data is decrypted by the decrypt program.\n"
                        "\n"
                        "USAGE\n"
                        "   ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey\n"
                        "\n"
                        "OPTIONS\n"
                        "   -h              Display program help and usage.\n"
                        "   -v              Display verbose program output.\n"
                        "   -i infile       Input file of data to encrypt (default: stdin).\n"
                        "   -o outfile      Output file for encrypted data (default: stdout).\n"
                        "   -n pbfile       Public key file (default: rsa.pub).\n");
        return 0;
    }

    pbfile = fopen(pbfile_name, "r");
    if (pbfile == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        exit(1);
    }

    mpz_t n, e, s, user;
    mpz_inits(n, e, s, user, NULL);

    char username[1024];
    rsa_read_pub(n, e, s, username, pbfile);

    if (verbose) {
        gmp_printf("user = %s\n"
                   "s (%zu bits) = %Zd\n"
                   "n (%zu bits) = %Zd\n"
                   "e (%zu bits) = %Zd\n",
            username, mpz_sizeinbase(s, 2), s, mpz_sizeinbase(n, 2), n, mpz_sizeinbase(e, 2), e);
    }

    mpz_set_str(user, username, 62);
    if (rsa_verify(user, s, e, n) == false) {
        fprintf(stderr, "Failed to verify user.\n");
        exit(1);
    }

    rsa_encrypt_file(infile, outfile, n, e);

    fclose(infile);
    fclose(outfile);
    fclose(pbfile);
    mpz_clears(n, e, s, user, NULL);
    return 0;
}
