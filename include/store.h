#ifndef STORE_H
#define STORE_H

#include <gmp.h>
#include <stdio.h>
#include <string.h>

int save_values(const char *filename, const mpz_t p, const mpz_t q, const mpz_t n, const mpz_t phi, const mpz_t e, const mpz_t d);
int load_values(const char *filename, mpz_t p, mpz_t q, mpz_t n, mpz_t phi, mpz_t e, mpz_t d);

int save_keys(const char *filename, const mpz_t n, const mpz_t e, const mpz_t d);
int load_keys(const char *filename, mpz_t n, mpz_t e, mpz_t d);

#endif