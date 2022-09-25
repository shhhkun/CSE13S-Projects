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

static char *pvfile_name = "rsa.priv";

int main(int argc, char **argv) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pvfile;
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
            pvfile_name = optarg;
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
                        "   Decrypts data using RSA decryption.\n"
                        "   Encrypted data is encrypted by the encrypt program.\n"
                        "\n"
                        "USAGE\n"
                        "   ./decrypt [-hv] [-i infile] [-o outfile] -n privkey\n"
                        "\n"
                        "OPTIONS\n"
                        "   -h              Display program help and usage.\n"
                        "   -v              Display verbose program output.\n"
                        "   -i infile       Input file of data to decrypt (default: stdin).\n"
                        "   -o outfile      Output file for decrypted data (default: stdout).\n"
                        "   -n pvfile       Private key file (default: rsa.priv).\n");
        return 0;
    }

    pvfile = fopen(pvfile_name, "r");
    if (pvfile == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        exit(1);
    }

    mpz_t n, d;
    mpz_inits(n, d, NULL);

    rsa_read_priv(n, d, pvfile);

    if (verbose) {
        gmp_printf("n (%zu bits) = %Zd\n"
                   "d (%zu bits) = %Zd\n",
            mpz_sizeinbase(n, 2), n, mpz_sizeinbase(d, 2), d);
    }

    rsa_decrypt_file(infile, outfile, n, d);

    fclose(infile);
    fclose(outfile);
    fclose(pvfile);
    mpz_clears(n, d, NULL);
    return 0;
}
