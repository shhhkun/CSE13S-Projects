#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>

#define OPTIONS "hvb:c:n:d:s:"

static bool header = false;
static bool verbose = false;
static bool default_time = true;
static uint64_t bits = 256;
static uint64_t confidence = 50;
static uint64_t seed;

static char *pbfile_name = "rsa.pub";
static char *pvfile_name = "rsa.priv";

int main(int argc, char **argv) {
    FILE *pbfile;
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
        case 'b': {
            bits = atoi(optarg);
            break;
        }
        case 'c': {
            confidence = atoi(optarg);
            break;
        }
        case 'n': {
            pbfile_name = optarg;
            break;
        }
        case 'd': {
            pvfile_name = optarg;
            break;
        }
        case 's': {
            default_time = false;
            seed = atoi(optarg);
            break;
        }
        default: {
            header = true;
            break;
        }
        }
    }
    if (header) {
        fprintf(stderr,
            "SYNOPSIS\n"
            "   Generates an RSA public/private key pair.\n"
            "\n"
            "USAGE\n"
            "   ./keygen [-hv] [-b bits] -n pbfile -d pvfile\n"
            "\n"
            "OPTIONS\n"
            "   -h              Display program help and usage.\n"
            "   -v              Display verbose program output.\n"
            "   -b bits         Minimum bits needed for public key n (default: 256).\n"
            "   -c confidence   Miller-Rabin iterations for testing primes (default: 50).\n"
            "   -n pbfile       Public key file (default: rsa.pub).\n"
            "   -d pvfile       Private key file (default: rsa.priv).\n"
            "   -s seed         Random seed for testing.\n");
        return 0;
    }

    if (default_time) {
        seed = time(NULL); // time since UNIX epoch
    }

    pbfile = fopen(pbfile_name, "w");
    pvfile = fopen(pvfile_name, "w");
    if (pbfile == NULL || pvfile == NULL) {
        fprintf(stderr, "Failed to open file(s).\n");
    }
    fchmod(fileno(pvfile), 0600); // set pv file perms to "0600"

    randstate_init(seed); // initialize state

    mpz_t p, q, n, e, d, user, s;
    mpz_inits(p, q, n, e, d, user, s, NULL);

    rsa_make_pub(p, q, n, e, bits, confidence); // make pub and priv keys
    rsa_make_priv(d, e, p, q);

    char *username = getenv("USER"); // RSA signing
    mpz_set_str(user, username, 62);
    rsa_sign(s, user, d, n);

    rsa_write_pub(n, e, s, username, pbfile); // write pub and priv keys
    rsa_write_priv(n, d, pvfile);

    if (verbose) {
        gmp_printf("user = %s\n"
                   "s (%zu bits) = %Zd\n"
                   "p (%zu bits) = %Zd\n"
                   "q (%zu bits) = %Zd\n"
                   "n (%zu bits) = %Zd\n"
                   "e (%zu bits) = %Zd\n"
                   "d (%zu bits) = %Zd\n",
            username, mpz_sizeinbase(s, 2), s, mpz_sizeinbase(p, 2), p, mpz_sizeinbase(q, 2), q,
            mpz_sizeinbase(n, 2), n, mpz_sizeinbase(e, 2), e, mpz_sizeinbase(d, 2), d);
    }

    fclose(pbfile);
    fclose(pvfile);
    randstate_clear(); // clear the state
    mpz_clears(p, q, n, e, d, user, s, NULL);
    return 0;
}
