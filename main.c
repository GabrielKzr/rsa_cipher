#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include "gen_keys.h"
#include "store.h"
#include "crypt.h"

int main(void) {
    
    gmp_randstate_t st;
    gmp_randinit_default(st);

    // ATENCAO: para uso serio, sementeie com /dev/urandom (veja nota abaixo)
    gmp_randseed_ui(st, (unsigned long) time(NULL));
    mpz_t p, q, n, e, phi, d;
    mpz_inits(p, q, n, phi, e, d, NULL);

    /*
    const mp_bitcnt_t k = 1024;
    const int reps = 30;
    
    // printf("======= GERANDO P, Q, N ========\n");
    gen_p_q_n(p, q, n, st, k, reps);
    // printf("======= GERANDO PHI ========\n");
    phi_euler(p, q, phi);
    // printf("======= GERANDO e ========\n");
    gen_e(e, phi, st);
    // printf("======= GERANDO d ========\n");
    gen_d(d, e, phi);
    
    
    printf("======= DEBUGANDO VALORES ========\n");
    */
    
    // printf("bitlen(n)=%zu\n", mpz_sizeinbase(n, 2)); // deve imprimir 2048
    
    // Imprime p, q e n em hexadecimal
    // gmp_printf("p   (hex) = 0x%Zx\n", p);
    // gmp_printf("q   (hex) = 0x%Zx\n", q);
    // gmp_printf("n   (hex) = 0x%Zx\n", n);
    // gmp_printf("phi (hex) = 0x%Zx\n", phi);
    // gmp_printf("e   (hex) = 0x%Zx   \n", e);
    // gmp_printf("d   (hex) = 0x%Zx   \n", d);
    
    // int ret = save_values("files/values.txt", p, q, n, phi, e, d);

    if (load_values("files/values.txt", p, q, n, phi, e, d) != 0) {
        fprintf(stderr, "Erro ao carregar valores de values.txt\n");
        mpz_clears(p, q, n, phi, e, d, NULL);
        gmp_randclear(st);
        return -1;
    }
   
    
    int ret = encrypt("files/arquivo.txt", "files/arquivo_novo.txt", e, n);
    
    if(ret < 0) {
        printf("ERRO ao encriptar"); 
        return -1;
    }

    ret = decrypt("files/arquivo_novo.txt", "files/arquivo_novo_novo.txt", d, n);

    if(ret < 0) {
        printf("ERRO ao decriptar"); 
        return -1;
    }

    mpz_clears(p, q, n, phi, e, d, NULL);
    gmp_randclear(st);

    return 0;
}


// RSA algorithm test
/*
mpz_set_ui(p, 61);
mpz_set_ui(q, 53);
mpz_mul(n, p, q);            // n = p * q
phi_euler(p, q, phi);       // phi = (p-1)*(q-1)
mpz_set_ui(e, 17);          // expositor público
gen_d(d, e, phi);             // d = e^{-1} mod phi

size_t mod_bytes = (mpz_sizeinbase(n, 2) + 7) / 8;
size_t plain_max = mod_bytes - 1; // seguro sem padding

uint8_t plain[plain_max];
// uint8_t plain2[plain_max];
uint8_t respo[plain_max];
uint8_t respo2[plain_max];
memset(plain, 0, plain_max);
memset(respo, 0, plain_max);
// memset(plain2, 0, plain_max);
memset(respo2, 0, plain_max);

plain[0] = 'a';


printf("plain_max -> %zu\n", plain_max);
printf("plain -> %s\n", plain);


mpz_t x1, x2, c, y;
mpz_inits(x1, x2, c, y, NULL);

mpz_import(x1, plain_max, -1, 1, 0, 0, plain);
gmp_printf("n -> %Zd\n", n);
gmp_printf("x -> %Zd\n", x1);

mpz_powm(c, x1, e, n);

size_t respo_size = 0;
mpz_export(respo, &respo_size, -1, 1, 0, 0, c);

mpz_import(x2, respo_size, -1, 1, 0, 0, respo);

mpz_powm(y, x2, d, n);

gmp_printf("y -> %Zd\n", y);

size_t respo_size2 = 0;
mpz_export(respo2, &respo_size2, -1, 1, 0, 0, y);
respo2[respo_size2] = 0;

printf("respo -> %s\n", respo2);
*/