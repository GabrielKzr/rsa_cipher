#ifndef CRYPT_H
#define CRYPT_H

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"

#define N_SIZE 256

int encrypt(const char* filepath_in, const char* filepath_out, const mpz_t e, const mpz_t n);
int decrypt(const char* filepath_in, const char* filepath_out, const mpz_t d, const mpz_t n);
int crypt_eq(const mpz_t exp, const mpz_t mod, const uint8_t* buf_in, size_t in_len, uint8_t* buf_out, size_t *out_len);

#endif