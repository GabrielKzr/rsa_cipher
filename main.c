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
    
    printf("======= GERANDO P, Q, N ========\n");
    gen_p_q_n(p, q, n, st, k, reps);
    printf("======= GERANDO PHI ========\n");
    phi_euler(p, q, phi);
    printf("======= GERANDO e ========\n");
    gen_e(e, phi, st);
    printf("======= GERANDO d ========\n");
    gen_d(d, e, n);
    
    
    printf("======= DEBUGANDO VALORES ========\n");
    
    printf("bitlen(n)=%zu\n", mpz_sizeinbase(n, 2)); // deve imprimir 2048
    
    // Imprime p, q e n em hexadecimal
    gmp_printf("p   (hex) = 0x%Zx\n", p);
    gmp_printf("q   (hex) = 0x%Zx\n", q);
    gmp_printf("n   (hex) = 0x%Zx\n", n);
    gmp_printf("phi (hex) = 0x%Zx\n", phi);
    gmp_printf("e   (hex) = 0x%Zx   \n", e);
    gmp_printf("d   (hex) = 0x%Zx   \n", d);
    
    
    printf("====== salvando valores =======\n");
    
    int ret = save_values("files/values.txt", p, q, n, phi, e, d);
    
    printf("====== valores salvos =======\n");
    
    if(ret < 0) return -1;
    */

    if (load_values("files/values.txt", p, q, n, phi, e, d) != 0) {
        fprintf(stderr, "Erro ao carregar valores de values.txt\n");
        mpz_clears(p, q, n, phi, e, d, NULL);
        gmp_randclear(st);
        return -1;
    }

    //printf("======== valores carregados =========");
    
    //printf("bitlen(n)=%zu\n", mpz_sizeinbase(n, 2)); // deve imprimir 2048
    
    // Imprime p, q e n em hexadecimal
    //gmp_printf("p   (hex) = 0x%Zx\n", p);
    //gmp_printf("q   (hex) = 0x%Zx\n", q);
    //gmp_printf("phi (hex) = 0x%Zx\n", phi);
    //gmp_printf("e   (hex) = 0x%Zx   \n", e);
    //gmp_printf("d   (hex) = 0x%Zx   \n", d);

    int ret = encrypt("files/arquivo.txt", "files/arquivo_novo.txt", e, n);

    if(ret < 0) {
        printf("ERRO ao encriptar"); 
        return -1;
    }

    mpz_clears(p, q, n, phi, e, d, NULL);
    gmp_randclear(st);

    return 0;
}
