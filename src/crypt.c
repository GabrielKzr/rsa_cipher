#include "crypt.h"

// AINDA PRECISA SER IMPLEMENTADO

static int crypt_eq(const mpz_t exp, const mpz_t mod,
                    const uint8_t* buf_in, size_t in_len,
                    uint8_t* buf_out, size_t *out_len)
{
    if (!buf_in || !buf_out || !out_len) return -1;

    mpz_t x, y;
    mpz_inits(x, y, NULL);

    // importa bytes (most significant byte first)
    mpz_import(x, in_len, -1, 1, 0, 0, buf_in);

    // mensagem deve ser < mod (n)
    if (mpz_cmp(x, mod) >= 0) {
        mpz_clears(x, y, NULL);
        return -2;
    }

    
    
    gmp_printf("Valor de x = %Zd\n", x);
    mpz_set(y, x);
    // mpz_powm(y, x, exp, mod);



    // exporta resultado; out_len recebe número de bytes escritos
    mpz_export(buf_out, out_len, -1, 1, 0, 0, y);

    mpz_clears(x, y, NULL);
    return 0;
}

int encrypt(const char* filepath_in, const char* filepath_out, const mpz_t e, const mpz_t d, const mpz_t n) {
    
    if (!filepath_in || !filepath_out) return -1;

    FILE *fin = fopen(filepath_in, "rb"); 
    if (!fin) return -2;    

    FILE *fout = fopen(filepath_out, "wb"); 
    if (!fout) { fclose(fin); return -3; }

    // tamanho máximo (bytes) para representar n
    size_t mod_bytes = (mpz_sizeinbase(n, 2) + 7) / 8;
    if (mod_bytes == 0) { fclose(fin); fclose(fout); return -4; }

    uint8_t *plain = malloc(mod_bytes);
    uint8_t *cipher = malloc(mod_bytes);
    uint8_t *dec = malloc(mod_bytes);
    if (!plain || !cipher || !dec) {
        free(plain); free(cipher); free(dec);
        fclose(fin); fclose(fout);
        return -5;
    }

    memset(plain, 0, 256);
    memset(cipher, 0, 256);
    memset(dec, 0, 256);

    size_t r;
    printf("começando...\n");
    while ((r = fread(plain, 1, mod_bytes - 1, fin)) > 0) {
        // uso mod_bytes-1 para garantir plain < n (sem padding). Para produção, usar padding adequado.
        if (r >= mod_bytes) {
            fprintf(stderr, "Bloco maior que modulo; implementar segmentação/padding\n");
            free(plain); free(cipher); free(dec);
            fclose(fin); fclose(fout);
            return -6;
        }

        size_t cipher_len = 0;
        int rc = crypt_eq(e, n, plain, r, cipher, &cipher_len);
        if (rc != 0) {
            fprintf(stderr, "Erro ao encriptar: %d\n", rc);
            free(plain); free(cipher); free(dec);
            fclose(fin); fclose(fout);
            return -7;
        }

        // grava ciphertext no ficheiro (escrever cipher_len bytes)
        if (fwrite(cipher, 1, cipher_len, fout) != cipher_len) {
            perror("fwrite");
            free(plain); free(cipher); free(dec);
            fclose(fin); fclose(fout);
            return -8;
        }

        size_t dec_len = 0;
        int rc2 = crypt_eq(d, n, cipher, cipher_len, dec, &dec_len);
        if(rc2 == 0) {
            printf("dec -> %s\n", (char*)dec);
        } else {
            printf("DEU MERDA");
        }
    }

    free(plain);
    free(cipher);
    free(dec);
    fclose(fin);
    fclose(fout);
    return 0;
}

int decrypt(const char* filepath_in, const char* filepath_out, const mpz_t d, const mpz_t n) {

    return 0;
}