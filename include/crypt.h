#ifndef CRYPT_H
#define CRYPT_H

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "packet.h"

int encrypt(const char* filepath_in, const char* filepath_out, const mpz_t e, const mpz_t n);
int decrypt(const char* filepath_in, const char* filepath_out, const mpz_t d, const mpz_t n);

#endif