#include "store.h"

int save_values(const char *filename,
                       const mpz_t p, const mpz_t q, const mpz_t n,
                       const mpz_t phi, const mpz_t e, const mpz_t d)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        perror("fopen");
        return -1;
    }

    if (mpz_out_str(f, 16, p) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }
    if (mpz_out_str(f, 16, q) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }
    if (mpz_out_str(f, 16, n) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }
    if (mpz_out_str(f, 16, phi) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }
    if (mpz_out_str(f, 16, e) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }
    if (mpz_out_str(f, 16, d) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }

    fclose(f);
    return 0;
}

int load_values(const char *filename,
                       mpz_t p, mpz_t q, mpz_t n,
                       mpz_t phi, mpz_t e, mpz_t d)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("fopen");
        return -1;
    }

    if (mpz_inp_str(p, f, 16) == 0) { fclose(f); return -2; }
    if (mpz_inp_str(q, f, 16) == 0) { fclose(f); return -2; }
    if (mpz_inp_str(n, f, 16) == 0) { fclose(f); return -2; }
    if (mpz_inp_str(phi, f, 16) == 0) { fclose(f); return -2; }
    if (mpz_inp_str(e, f, 16) == 0) { fclose(f); return -2; }
    if (mpz_inp_str(d, f, 16) == 0) { fclose(f); return -2; }

    fclose(f);
    return 0;
}

int save_keys(const char *filename, const mpz_t n, const mpz_t e, const mpz_t d)
{
    if (!filename || !n || !e || !d) return -1;
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        return -1;
    }

    if (fprintf(f, "RSA\n") < 0) { fclose(f); return -2; }
    if (mpz_out_str(f, 16, n) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }
    if (mpz_out_str(f, 16, e) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }
    if (mpz_out_str(f, 16, d) < 0 || fputc('\n', f) == EOF) { fclose(f); return -2; }

    fclose(f);
    return 0;
}

int load_keys(const char *filename, mpz_t n, mpz_t e, mpz_t d)
{
    if (!filename || !n || !e || !d) return -1;
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return -1;
    }

    char header[8] = {0};
    if (fgets(header, sizeof(header), f) == NULL) { fclose(f); return -2; }

    if (strncmp(header, "RSA", 3) != 0) {
        fclose(f);
        return -3;
    }

    if (mpz_inp_str(n, f, 16) == 0) { fclose(f); return -4; }
    if (mpz_inp_str(e, f, 16) == 0) { fclose(f); return -4; }
    if (mpz_inp_str(d, f, 16) == 0) { fclose(f); return -4; }

    fclose(f);
    return 0;
}