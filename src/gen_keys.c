#include "gen_keys.h"

static void rand_kbit_odd(mpz_t r, gmp_randstate_t st, mp_bitcnt_t k) {
    // Gera r uniforme em, forca exatamente k bits e impar
    mpz_urandomb(r, st, k); // <<< trocado de mpz_rrandomb para mpz_urandomb
    mpz_setbit(r, k - 1); // garante bit mais significativo -> exatamente k bits
    mpz_setbit(r, 0); // garante impar
}

static void gen_prime_kbits(mpz_t p, gmp_randstate_t st, mp_bitcnt_t k, int reps) {
    do {
        rand_kbit_odd(p, st, k);
        mpz_nextprime(p, p); // sobe para o proximo primo >= candidato (provavel primo)
    // Se por acaso passou do limite de k bits, tenta de novo
    } while (mpz_sizeinbase(p, 2) != k || mpz_probab_prime_p(p, reps) == 0);
}

void gen_p_q_n(mpz_t p, mpz_t q, mpz_t n, gmp_randstate_t st, mp_bitcnt_t k, int reps) {
    do {
        gen_prime_kbits(p, st, k, reps);
        do {
            gen_prime_kbits(q, st, k, reps);
        } while (mpz_cmp(p, q) == 0); // garante p != q
        mpz_mul(n, p, q);
    } while (mpz_sizeinbase(n, 2) != 2048);
}

void phi_euler(mpz_t p, mpz_t q, mpz_t phi) {
    mpz_t p1, q1;

    mpz_inits(p1, q1, NULL);

    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);

    mpz_mul(phi, p1, q1);

    mpz_clears(p1, q1, NULL);
}

void gen_e(mpz_t e, const mpz_t phi, gmp_randstate_t st) {
    mpz_t gcd, limit;

    mpz_inits(gcd, limit, NULL);
    mpz_sub_ui(limit, phi, 1);

    do {
        mpz_urandomm(e, st, limit);

        if (mpz_cmp_ui(e, 2) < 0)
            mpz_set_ui(e, 2);

        mpz_gcd(gcd, e, phi);
        // gmp_printf("gcd(e, phi) = %Zd\n", gcd);

    } while (mpz_cmp_ui(gcd, 1) != 0);  // repete se gcd(e, phi) != 1

    mpz_clears(gcd, limit, NULL);
}

void gen_d(mpz_t d, mpz_t e, mpz_t phi) {
    mpz_invert(d, e, phi);
}