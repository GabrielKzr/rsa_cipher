#include "crypt.h"

static int crypt_eq(const mpz_t exp, const mpz_t mod,
                    const uint8_t* buf_in, size_t in_len,
                    uint8_t* buf_out, size_t *out_len)
{
    if (!buf_in || !buf_out || !out_len) return -1;

    mpz_t x, y;
    mpz_inits(x, y, NULL);

    mpz_import(x, in_len, -1, 1, 0, 0, buf_in);

    // mensagem deve ser < mod (n)
    if (mpz_cmp(x, mod) >= 0) {
        mpz_clears(x, y, NULL);
        return -2;
    }
    
    mpz_powm(y, x, exp, mod);

    mpz_export(buf_out, out_len, -1, 1, 0, 0, y);

    mpz_clears(x, y, NULL);
    return 0;
}

int encrypt(const char* filepath_in, const char* filepath_out, const mpz_t e, const mpz_t n) {
    
    if (!filepath_in || !filepath_out) return -1;

    FILE *fin = fopen(filepath_in, "rb"); 
    if (!fin) return -2;    

    FILE *fout = fopen(filepath_out, "wb"); 
    if (!fout) { fclose(fin); return -3; }

    uint8_t *plain = malloc(MAX_PAYLOAD);
    if (!plain) {
        free(plain);
        fclose(fin); fclose(fout);
        return -5;
    }

    memset(plain, 0, MAX_PAYLOAD);

    size_t r;
    printf("começando...\n");
    printf("sizeof(packet) -> %zu\n", sizeof(packet));

    packet*  p = malloc(sizeof(packet));
    uint8_t* c = malloc(256);
    
    while ((r = fread(plain, 1, MAX_PAYLOAD, fin)) > 0) {
        
        // limpar
        memset(p, 0, sizeof(packet));
        memset(c, 0, 256);
        size_t cipher_len = 0;

        // empacotar
        create_packet(plain, r, p);

        //  encriptar
        int ret = crypt_eq(e, n, p->bytes, MAX_FRAMES, c, &cipher_len);
        if (ret != 0) {
            fprintf(stderr, "Erro na criptografia: %d\n", ret);
            return ret;
        }

        // salvar
        if (fwrite(c, 1, cipher_len, fout) != cipher_len) {
            perror("Erro ao gravar arquivo de saída");
            return -1;
        }
    }

    free(p);
    free(c);
    free(plain);
    fclose(fin);
    fclose(fout);
    return 0;
}

int decrypt(const char* filepath_in, const char* filepath_out, const mpz_t d, const mpz_t n) {

    if (!filepath_in || !filepath_out) return -1;

    FILE *fin = fopen(filepath_in, "rb"); 
    if (!fin) return -2;    

    FILE *fout = fopen(filepath_out, "wb"); 
    if (!fout) { fclose(fin); return -3; }   

    uint8_t *plain = malloc(N_SIZE);
    if (!plain) {
        free(plain);
        fclose(fin); fclose(fout);
        return -5;
    }

    memset(plain, 0, N_SIZE);
    
    size_t r;
    printf("começando...\n");
    printf("sizeof(packet) -> %zu\n", sizeof(packet));

    packet*  p = malloc(sizeof(packet));
    uint8_t* s = malloc(MAX_PAYLOAD);

    while ((r = fread(plain, 1, N_SIZE, fin)) > 0) {
        
        // limpar
        memset(p, 0, sizeof(packet));
        memset(s, 0, MAX_PAYLOAD);
        size_t data_len = 0;

        // decriptar
        int ret = crypt_eq(d, n, plain, N_SIZE, p->bytes, &data_len);
        if (ret != 0) {
            fprintf(stderr, "Erro na descriptografia: %d\n", ret);
            return ret;
        }

        // desempacotar
        unpack_packet(p, s);

        // salvar
        if (fwrite(s, 1, data_len-1, fout) != data_len-1) {
            perror("Erro ao gravar arquivo de saída");
            return -1;
        }
    }

    free(p);
    free(s);
    free(plain);
    fclose(fin);
    fclose(fout);

    return 0;
}