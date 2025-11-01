#ifndef GEN_KEYS_H
#define GEN_KEYS_H

#include <gmp.h>
#include <time.h>

void gen_p_q_n(mpz_t p, mpz_t q, mpz_t n, gmp_randstate_t st, mp_bitcnt_t k, int reps);
void phi_euler(mpz_t p, mpz_t q, mpz_t phi);
void gen_e(mpz_t e, const mpz_t phi, gmp_randstate_t st);
void gen_d(mpz_t d, mpz_t e, mpz_t phi);
void gen_keys(mpz_t p, mpz_t q, mpz_t n, mpz_t phi, mpz_t e, mpz_t d);

#endif