#include "tests.h"

int test1() {
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

int testImage() {

    mpz_t p, q, n, e, phi, d;
    mpz_inits(p, q, n, phi, e, d, NULL);

    if (load_values("files/values.txt", p, q, n, phi, e, d) != 0) {
        fprintf(stderr, "Erro ao carregar valores de values.txt\n");
        mpz_clears(p, q, n, phi, e, d, NULL);
        return -1;
    }

    if (load_values("files/values.txt", p, q, n, phi, e, d) != 0) {
        fprintf(stderr, "Erro ao carregar valores de values.txt\n");
        mpz_clears(p, q, n, phi, e, d, NULL);
        return -1;
    }
   
    
    int ret = encrypt("files/Diagrama.jpeg", "files/save.bin", e, n);
    
    if(ret < 0) {
        printf("ERRO ao encriptar"); 
        return -1;
    }

    ret = decrypt("files/save.bin", "files/image_teste.jpeg", d, n);
    
    if(ret < 0) {
        printf("ERRO ao decriptar"); 
        return -1;
    }

    printf("terminando...\n");

    mpz_clears(p, q, n, phi, e, d, NULL);   

    return 0;
}

int testPacket() {

    uint8_t data_in[MAX_PAYLOAD-1];
    packet* pa = malloc(sizeof(packet));

    for(int i = 0; i < MAX_PAYLOAD-1; i++) {
        data_in[i] = i;
    }

    create_packet(data_in, MAX_PAYLOAD-1, pa);

    print_packet_hex(pa);

    // ------------- testar mpz_t

    mpz_t x, y;
    mpz_inits(x, y, NULL);

    mpz_import(x, MAX_FRAMES, 1, 1, 0, 0, pa->bytes);
    mpz_import(y, MAX_FRAMES, -1, 1, 0, 0, pa->bytes);

    gmp_printf("x -> %Zd\n", x); // usar o caso do x
    gmp_printf("y -> %Zd\n", y);

    mpz_clears(x, y, NULL);

    // ------------- testar função de encript

    mpz_t p, q, n, e, phi, d;
    mpz_inits(p, q, n, phi, e, d, NULL);

    load_values("files/values.txt", p, q, n, phi, e, d);

    uint8_t temp_out[MAX_FRAMES] = {0};
    uint8_t resp_out[MAX_FRAMES] = {0};
    size_t size_out;    

    // print_packet_hex(pa);

    if(crypt_eq(e, n, pa->bytes, MAX_FRAMES, temp_out, &size_out) != 0) printf("ERRO1\n");

    printf("size_out -> %zu\n", size_out);

    if(crypt_eq(d, n, temp_out, size_out, resp_out, NULL) != 0) printf("ERRO2");

    printf("printando out...\n");
    print_packet_hex((packet*)resp_out);

    mpz_clears(p, q, n, phi, e, d, NULL);   

    // --------------------------- testando o unpack

    uint8_t temp[MAX_PAYLOAD-1] = {0};

    printf("packet...\n");
    print_packet_hex(pa);

    unpack_packet(pa, temp);

    printf("unpacking...\n");

    print_packet_hex((packet*)temp);

    free(pa);

    // tudo funcionando aqui

    return 0;
}

