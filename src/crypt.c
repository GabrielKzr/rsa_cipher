#include "crypt.h"

// AINDA PRECISA SER IMPLEMENTADO

int encrypt(const char* filepath_in, const char* filepath_out, const mpz_t e, const mpz_t n) {
    
    if (!filepath_in || !filepath_out) return -1;

    FILE *fin = fopen(filepath_in, "rb"); 
    if (!fin) return -2;    

    FILE *fout = fopen(filepath_out, "wb"); 
    if (!fout) { fclose(fin); return -3; }

    uint8_t *buf = malloc(MAX_FRAMES);
    if (!buf) { fclose(fin); fclose(fout); return -4; }

    size_t r;

    printf("começando...\n");

    while ((r = fread(buf, 1, MAX_FRAMES, fin)) > 0) {

        // TODO: substituir este fwrite pela encriptação real

        printf("valor do r -> %zu\n", r);

        if (fwrite(buf, 1, r, fout) != r) {
            free(buf);
            fclose(fin);
            fclose(fout);
            return -5;
        }
    }
    
    return 0;
}


int decrypt(const char* filepath_in, const char* filepath_out, const mpz_t d, const mpz_t n) {

    return 0;
}