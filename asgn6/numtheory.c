#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>

#include "numtheory.h"
#include "randstate.h"

static bool inv_exists = true;
static bool prime_found = false;

// Usage: This function will compute the gcd (greatest common divisor) between two mpz_t numbers 'a' and 'b'.
//
// Parameters: two numbers 'a' and 'b', and 'd' where the result is passed.
// Return value: void.
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t t, tmp_a, tmp_b;
    mpz_inits(t, tmp_a, tmp_b, NULL);

    mpz_set(tmp_a, a); // placeholder for a
    mpz_set(tmp_b, b); // placeholder for b
    while (mpz_cmp_ui(tmp_b, 0) != 0) {
        mpz_set(t, tmp_b); // t <- b
        mpz_mod(tmp_b, tmp_a, tmp_b); // b <- a mod b
        mpz_set(tmp_a, t); // a <- t
    }
    mpz_set(d, tmp_a); // g <- a (the stored gcd of a and b)
    mpz_clears(t, tmp_a, tmp_b, NULL);
    return;
}

// Usage: This function will compute the modular inverse between two mpz_t numbers 'a' and 'n'.
//
// Parameters: two numbers 'a' and 'n', and 'i' where the result is passed.
// Return value: void.
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r, tmp_r, r_1, tmp_r1, t, tmp_t, t_1, tmp_t1, q, qr1, qt1;
    mpz_inits(r, tmp_r, r_1, tmp_r1, t, tmp_t, t_1, tmp_t1, q, qr1, qt1, NULL);

    mpz_set(r, n); // r <- n
    mpz_set(r_1, a); // r_1 <- a
    mpz_set_ui(t, 0); // t <- 0
    mpz_set_ui(t_1, 1); // t_1 <- 1

    while (mpz_cmp_ui(r_1, 0) != 0) {
        mpz_fdiv_q(q, r, r_1); // q <- floor(r / r_1)

        mpz_set(tmp_r, r); // temp values for r and r_1
        mpz_set(tmp_r1, r_1);

        mpz_set(r, tmp_r1); // r <- tmp r1
        mpz_mul(qr1, q, tmp_r1); // qr1 <- q * r_1
        mpz_sub(r_1, tmp_r, qr1); // r_1 <- tmp_r - qr1

        mpz_set(tmp_t, t); // temp values for t and t_1
        mpz_set(tmp_t1, t_1);

        mpz_set(t, tmp_t1); // t <- tmp_t1
        mpz_mul(qt1, q, tmp_t1); // qt1 <- q * tmp_t1
        mpz_sub(t_1, tmp_t, qt1); // t_1 <- tmp_t - qt1
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        inv_exists = false;
        mpz_set_ui(i, 0); // no inverse
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n); // t <- t + n
    }
    if (inv_exists) {
        mpz_set(i, t); // i <- t (pass inverse t to i)
    }
    inv_exists = true; // reset bool
    mpz_clears(r, tmp_r, r_1, tmp_r1, t, tmp_t, t_1, tmp_t1, q, qr1, qt1, NULL);
}

// Usage: This function will perform exponent modulus with base 'base', the exponent 'exponent'
//        , and modulus 'modulus'.
//
// Parameters: 'base', 'exponent', 'modulus', and 'out' where the result is passed.
// Return value: void.
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t v, p, vp, vp_mod, pp, pp_mod, f_e, tmp_e;
    mpz_inits(v, p, vp, vp_mod, pp, pp_mod, f_e, tmp_e, NULL);

    mpz_set_ui(v, 1); // v <- 1
    mpz_set(p, base); // p <- base
    mpz_set(tmp_e, exponent); // tmp_e <- e (temp value for exponent)

    while (mpz_sgn(tmp_e) == 1) { // while exponent > 0
        if (mpz_odd_p(tmp_e) != 0) {
            mpz_mul(vp, v, p); // vp <- v * p
            mpz_mod(vp_mod, vp, modulus);
            mpz_set(v, vp_mod); // v <- vp mod n
        }
        mpz_mul(pp, p, p); // pp <- p * p
        mpz_mod(pp_mod, pp, modulus);
        mpz_set(p, pp_mod); // p <- pp mod n
        mpz_fdiv_q_ui(f_e, tmp_e, 2);
        mpz_set(tmp_e, f_e); // tmp_e <- floor(tmp_e, 2)
    }
    mpz_set(out, v);
    mpz_clears(v, p, vp, vp_mod, pp, pp_mod, f_e, tmp_e, NULL);
    return;
}

// Usage: This function using the Rabin-Miller primality test will check whether the mpz_t 'n' is a prime number.
//
// Parameters: a number 'n' and an amount of iterations to run the priamlity test for.
// Return value: a boolean indicating whether 'n' is prime or not.
bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t r, s, i, rand, a, y, j, two, n_1, n_3;
    mpz_inits(r, s, i, rand, a, y, j, two, n_1, n_3, NULL); // inits to default 0

    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) { // if n is 2 or 3
        mpz_clears(r, s, i, rand, a, y, j, two, n_1, n_3, NULL);
        return true;
    }

    if (mpz_cmp_ui(n, 1) <= 0 || mpz_even_p(n) != 0) { // if n <= 1 or n is even
        mpz_clears(r, s, i, rand, a, y, j, two, n_1, n_3, NULL);
        return false;
    }

    mpz_set_ui(s, 0); // s <- 0
    mpz_sub_ui(r, n, 1); // r <- n - 1
    mpz_set_ui(two, 2); // two <- 2

    while (mpz_even_p(r) != 0) { // while r is even
        mpz_add_ui(s, s, 1); // s += 1
        mpz_fdiv_q_ui(r, r, 2); // r //= 2
    }

    mpz_sub_ui(n_1, n, 1); // n_1 <- n - 1
    mpz_sub_ui(n_3, n, 3); // n_3 <- n - 3

    for (mpz_set_ui(i, 1); mpz_cmp_ui(i, iters) <= 0; mpz_add_ui(i, i, 1)) { // for i to k
        mpz_urandomm(rand, state, n_3); // generate random number "rand" from a {0,...,n - 4}
        mpz_add_ui(a, rand, 2); // choosing a from a {2,3,...,n-2} (add 2)
        pow_mod(y, a, r, n); // y <- pow(a,r,n)
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_1) != 0) { // if y != 1 and y != n - 1
            mpz_set_ui(j, 1); // j <- 1
            while (mpz_cmp(j, s) < 0 && mpz_cmp(y, n_1) != 0) { // while j < s and y != n - 1
                pow_mod(y, y, two, n); // y <- y^2
                if (mpz_cmp_ui(y, 1) == 0) { // if y == 1
                    mpz_clears(r, s, i, rand, a, y, j, two, n_1, n_3, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1); // j <- j + 1
            }
            if (mpz_cmp(y, n_1) != 0) { // if y != n - 1
                mpz_clears(r, s, i, rand, a, y, j, two, n_1, n_3, NULL);
                return false;
            }
        }
    }
    mpz_clears(r, s, i, rand, a, y, j, two, n_1, n_3, NULL);
    return true;
}

// Usage: This function will generate a random prime number of at least bits length.
//
// Parameters: bits length of the prime number, a number of iterations, and 'p' where the prime number will be passed.
// Return value: void.
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t rand, min, one;
    mpz_inits(rand, min, one, NULL);

    mpz_set_ui(one, 1); // one <- 1
    mpz_mul_2exp(min, one, bits - 1); // min = 1 * 2^(bits - 1)
    while (!prime_found) {
        mpz_urandomb(rand, state, bits - 1); // range [0, 2^(bits - 1) - 1]
        mpz_add(rand, rand, min); // now range [2^(n - 1), (2^n) - 1]
        if (is_prime(rand, iters)) {
            mpz_set(p, rand); // p <- rand
            prime_found = true;
        }
    }
    prime_found = false;
    mpz_clears(rand, min, one, NULL);
}
