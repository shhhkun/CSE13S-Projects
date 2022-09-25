#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"

// Credit to Prof. Long for log computation algorithm.
//
// Usage: This function will compute the logarithm of a mpz_t number 'n', and produce its result in 'e'.
//
// Parameters: number 'n', and exponent 'e'
// Return value: void.
void lg(mpz_t n, mpz_t e) {
    mpz_t tmp_n, tmp_e;
    mpz_inits(tmp_n, tmp_e, NULL);

    mpz_abs(tmp_n, n); // n <- |n|
    mpz_set_ui(tmp_e, 0); // e <- 0
    while (mpz_cmp_ui(tmp_n, 0) > 0) {
        mpz_add_ui(tmp_e, tmp_e, 1); // e <- e + 1
        mpz_fdiv_q_ui(tmp_n, tmp_n, 2); // n <- n // 2
    }
    mpz_sub_ui(e, tmp_e, 1); // e <- e - 1
    mpz_clears(tmp_n, tmp_e, NULL);
    return;
}

// Usage: This function will generate a public key which consists of 'n' the public modulus, and 'e' the public exponent.
//        The public key will be computed by generating two large prime numbers 'p' and 'q', and multiplying them to get
//        'n'.
//
// Parameters: 'p' and 'q', holder for public 'n' and 'e', number of bits to share between 'p' and 'q', and an amount of iterations
// Return value: void.
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    mpz_t rand, p_1, q_1, g, t_n;
    mpz_inits(rand, p_1, q_1, g, t_n, NULL);

    srandom(time(NULL)); // add randomness to random()
    uint64_t min = nbits / 4;
    uint64_t max = (2 * min) + 1;
    uint64_t bits_for_p = 1 + ((random() % max) + min); // range [nbits / 4, (3 * nbits) / 4)
    uint64_t bits_for_q = 1 + nbits - bits_for_p;

    make_prime(p, bits_for_p, iters);
    make_prime(q, bits_for_q, iters);

    mpz_mul(n, p, q); // n = p * q
    mpz_sub_ui(p_1, p, 1); // p_1 <- p - 1
    mpz_sub_ui(q_1, q, 1); // q_1 <- q - 1
    mpz_mul(t_n, p_1, q_1); // φ(n) = (p - 1)(q - 1)

    do {
        mpz_urandomb(rand, state, nbits);
        gcd(g, rand, t_n); // gcd of rand and n
    } while (mpz_cmp_ui(g, 1) != 0); // do while gcd 'g' of rand and n is not 1

    mpz_set(e, rand); // e <- rand (random coprime number found)
    mpz_clears(rand, p_1, q_1, g, t_n, NULL);
    return;
}

// Usage: This function will write out the public key to pbfile.
//
// Parameters: public 'n' and 'e', signature 's', username, and pbfile.
// Return value: void.
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
    return;
}

// Usage: This function will read the contents of pbfile to get the public key.
//
// Parameters: public 'n' and 'e', signature 's', username, and pbfile.
// Return value: void.
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
    return;
}

// Usage: This function will generate a private key when given the public exponent 'e', and the two prime numbers
//        'p' and 'q'. The private key will be 'd'.
//
// Parameters: public 'e', 'p' and 'q', and 'd' to hold the private key.
// Return value: void.
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t t_n, p_1, q_1;
    mpz_inits(t_n, p_1, q_1, NULL);

    mpz_sub_ui(p_1, p, 1); // p_1 <- p - 1
    mpz_sub_ui(q_1, q, 1); // q_1 <- q - 1
    mpz_mul(t_n, p_1, q_1); // φ(n) = (p - 1)(q - 1)

    mod_inverse(d, e, t_n);
    mpz_clears(t_n, p_1, q_1, NULL);
    return;
}

// Usage: This function will write the contents of the private key to pvfile.
//
// Parameters: public modulus 'n', private key 'd', and pvfile.
// Return value: void.
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
    return;
}

// Usage: This function will read the contents of pvfile to get the private key.
//
// Parameters: public modulus 'n', private key 'd', and pvfile.
// Return value: void.
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

// Usage: This function will perform RSA encryption on message 'm'.
//
// Parameters: message 'm', public exponent 'e', public modulus 'n', and 'c' to hold the ciphertext.
// Return value: void.
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n); // c <- m^e (mod n)
    return;
}

// Usage: This function will use rsa_encrypt() on the contents of a file until it has fully been encrypted.
//
// Parameters: infile, outfile, public modulus 'n', public exponent 'e'.
// Return value: void.
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t exp, lg2_1, k, m, c;
    mpz_inits(exp, lg2_1, k, m, c, NULL);

    lg(n, exp); // e = log_2(n)
    mpz_sub_ui(lg2_1, exp, 1); // lg2_1 <- exp - 1
    mpz_fdiv_q_ui(k, lg2_1, 8); // k = floor(lg_2 / 8)
    uint8_t *buf = (uint8_t *) calloc(mpz_get_ui(k), sizeof(uint8_t));
    buf[0] = 0xFF; // prepend 0xFF to start of buffer

    uint64_t nbytes = mpz_get_ui(k) - 1; // read at most k - 1 bytes
    uint64_t j;
    do {
        j = fread(&buf[1], sizeof(uint8_t), nbytes, infile);
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, buf); // j + 1 to account for 0xFF
        rsa_encrypt(c, m, e, n);
        if (j > 0) {
            gmp_fprintf(outfile, "%Zx\n", c);
        }
    } while (j > 0);
    mpz_clears(exp, lg2_1, k, m, c, NULL);
    free(buf);
    buf = NULL;
    return;
}

// Usage: This function will perform RSA decryption on ciphertext 'c' to retrieve the original message 'm'.
//
// Parameters: ciphertext 'c', private key 'd', public modulus 'n', and 'm' to hold the original message.
// Return value: void.
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n); // m <- c^d (mod n)
    return;
}

// Usage: This function will use rsa_decrypt on the contents of a file until it has fully been decrypted.
//
// Parameters: infile, outfile, public modulus 'n', private key 'd'.
// Return value: void.
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t exp, lg2_1, k, m, c;
    mpz_inits(exp, lg2_1, k, m, c, NULL);

    lg(n, exp); // e = log_2(n)
    mpz_sub_ui(lg2_1, exp, 1); // lg2_1 <- exp - 1
    mpz_fdiv_q_ui(k, lg2_1, 8); // k = floor(lg_2 / 8)
    uint8_t *buf = (uint8_t *) calloc(mpz_get_ui(k), sizeof(uint8_t));

    uint64_t j;
    do {
        gmp_fscanf(infile, "%Zx\n", c);
        rsa_decrypt(m, c, d, n);
        mpz_export(buf, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(&buf[1], sizeof(uint8_t), j - 1, outfile);
    } while (!feof(infile));
    mpz_clears(exp, lg2_1, k, m, c, NULL);
    free(buf);
    buf = NULL;
    return;
}

// Usage: This function will perform RSA signing by taking a message 'm', public modulus 'n', and private key 'd'
//        to produce a signature 's'.
//
// Parameters: message 'm', private key 'd', public modulus 'n', and signature 's' to hold the result.
// Return value: void.
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n); // s <- m^d (mod n)
    return;
}

// Usage: This function will verify that the signature 's' and original message 'm' are the same.
//
// Parameters: signature 's', public exponent 'e', public modulus 'n', original message 'm'.
// Return value: void.
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n); // t <- s^e (mod n)
    if (mpz_cmp(t, m) == 0) { // if t == m
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}
