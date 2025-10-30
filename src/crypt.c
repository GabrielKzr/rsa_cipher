#include "crypt.h"

// AINDA PRECISA SER IMPLEMENTADO

static void crypt_eq(mpz_t x, mpz_t e, mpz_t m) {




}

int encrypt(const char* filepath_in, const char* filepath_out, const mpz_t e, const mpz_t n) {
    
    if (!filepath_in || !filepath_out) return -1;

    FILE *fin = fopen(filepath_in, "rb"); 
    if (!fin) return -2;    

    FILE *fout = fopen(filepath_out, "wb"); 
    if (!fout) { fclose(fin); return -3; }

    uint8_t *buf = malloc(MAX_FRAMES);
    uint8_t *buf2 = malloc(MAX_FRAMES);
    if (!buf) { fclose(fin); fclose(fout); return -4; }

    size_t r;

    printf("começando...\n");

    memset(buf, 0, MAX_FRAMES);
    memset(buf2, 0, MAX_FRAMES);

    mpz_t x;
    mpz_init(x);

    while ((r = fread(buf, 1, MAX_FRAMES, fin)) > 0) {

        // TODO: substituir este fwrite pela encriptação real

        printf("valor do r -> %zu\n", r);

        // printf("%c\n", buf[0]);

        mpz_import(x, r, -1, 1, 0, 0, buf);
        
        mpz_export(buf2, NULL, -1, 1, 0, 0, x);

        printf("buf2 -> %s\n", (char*)buf2);

        if (fwrite(buf, 1, r, fout) != r) {
            free(buf);
            fclose(fin);
            fclose(fout);
            return -5;
        }
    }

    free(buf2);
    return 0;
}


int decrypt(const char* filepath_in, const char* filepath_out, const mpz_t d, const mpz_t n) {

    return 0;
}