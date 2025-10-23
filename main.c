#include <stdio.h>
#include <gmp.h>
#include <time.h>

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

static void fi_euler(mpz_t p, mpz_t q, mpz_t out) {
    mpz_t p1, q1;

    mpz_inits(p1, q1, NULL);

    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);

    mpz_mul(out, p1, q1);

    mpz_clears(p1, q1, NULL);
}


// IMPLEMENTAR AINDA
static void gen_e(mpz_t e, const mpz_t fi) {

    mpz_set_ui(e, 65537); // valor comum para e

    mpz_t gcd;
    mpz_init(gcd);

    mpz_gcd(gcd, e, fi);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        fprintf(stderr, "Erro: e e fi(n) nao sao coprimos.\n");
        mpz_clear(gcd);
        return;
    }

    mpz_clear(gcd);
}


int main(void) {

    gmp_randstate_t st;
    gmp_randinit_default(st);

    // ATENCAO: para uso serio, sementeie com /dev/urandom (veja nota abaixo)
    gmp_randseed_ui(st, (unsigned long) time(NULL));
    mpz_t p, q, n;
    mpz_inits(p, q, n, NULL);

    const mp_bitcnt_t k = 1024;
    const int reps = 30;


    do {
        gen_prime_kbits(p, st, k, reps);
        do {
            gen_prime_kbits(q, st, k, reps);
        } while (mpz_cmp(p, q) == 0); // garante p != q
        mpz_mul(n, p, q);
    } while (mpz_sizeinbase(n, 2) != 2048);

    printf("bitlen(n)=%zu\n", mpz_sizeinbase(n, 2)); // deve imprimir 2048

    // Imprime p, q e n em hexadecimal
    gmp_printf("p (hex) = 0x%Zx\n", p);
    gmp_printf("q (hex) = 0x%Zx\n", q);
    gmp_printf("n (hex) = 0x%Zx\n", n);

    mpz_clears(p, q, n, NULL);
    gmp_randclear(st);

    return 0;
}
