#include "handle_args.h"

const size_t num_commands = 3;
const char* commands[] = {"genkey", "encrypt", "decrypt"};
const char* options[] = {"--key", "--in", "--out"};
int (*handlers[])(int, char **) = {handle_genkey, handle_encrypt, handle_decrypt, handle_dummy};

int find_command(char* command) {
    for(size_t i = 0; i < num_commands; i++) {
        if(strcmp(command, commands[i]) == 0) return (int)i;
    }
    return (int)num_commands;
}

/* parse flags for encrypt/decrypt: returns 0 ok, -1 on error.
   keyfile/outfile defaulted if not provided; infile is required. */
static int parse_flags_common(int argc, char** argv, char** keyfile, char** infile, char** outfile) {
    *keyfile = "files/keys.txt";
    *outfile = "files/crypt_out.txt";
    *infile = NULL;

    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "--key") == 0) {
            if (i + 1 >= argc) { fprintf(stderr, "--key requires an argument\n"); return -1; }
            *keyfile = argv[++i];
        } else if (strcmp(argv[i], "--in") == 0) {
            if (i + 1 >= argc) { fprintf(stderr, "--in requires an argument\n"); return -1; }
            *infile = argv[++i];
        } else if (strcmp(argv[i], "--out") == 0) {
            if (i + 1 >= argc) { fprintf(stderr, "--out requires an argument\n"); return -1; }
            *outfile = argv[++i];
        } else {
            fprintf(stderr, "Unknown flag: %s\n", argv[i]);
            return -1;
        }
    }

    if (!*infile) { fprintf(stderr, "Missing required --in <file>\n"); return -1; }
    return 0;
}

/* parse flags for genkey: optional --key <file> */
static int parse_flags_genkey(int argc, char** argv, char** keyfile) {
    *keyfile = "files/keys.txt";
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "--key") == 0) {
            if (i + 1 >= argc) { fprintf(stderr, "--key requires an argument\n"); return -1; }
            *keyfile = argv[++i];
        } else {
            fprintf(stderr, "Unknown flag for genkey: %s\n", argv[i]);
            return -1;
        }
    }
    return 0;
}

int handle_genkey(int argc, char** argv) {
    char *keys_file = NULL;
    if (parse_flags_genkey(argc, argv, &keys_file) != 0) return -1;

    mpz_t p, q, n, e, phi, d;
    mpz_inits(p, q, n, phi, e, d, NULL);

    gen_keys(p, q, n, phi, e, d);

    if (save_values(keys_file, p, q, n, phi, e, d) != 0) {
        fprintf(stderr, "Failed to save keys to %s\n", keys_file);
        mpz_clears(p, q, n, phi, e, d, NULL);
        return -1;
    }

    printf("Keys saved to %s\n", keys_file);
    mpz_clears(p, q, n, phi, e, d, NULL);
    return 0;
}

int handle_encrypt(int argc, char** argv) {
    char *keyfile = NULL, *infile = NULL, *outfile = NULL;
    if (parse_flags_common(argc, argv, &keyfile, &infile, &outfile) != 0) return -1;

    if (access(keyfile, R_OK) != 0) { fprintf(stderr, "Key file not found or unreadable: %s\n", keyfile); return -1; }
    if (access(infile, R_OK) != 0)  { fprintf(stderr, "Input file not found or unreadable: %s\n", infile); return -1; }

    if (strcmp(keyfile, infile) == 0 || strcmp(keyfile, outfile) == 0 || strcmp(infile, outfile) == 0) {
        fprintf(stderr, "Filenames must be distinct: --key, --in and --out must be different\n");
        return -1;
    }

    mpz_t p, q, n, e, phi, d;
    mpz_inits(p, q, n, phi, e, d, NULL);

    if (load_values(keyfile, p, q, n, phi, e, d) != 0) {
        fprintf(stderr, "Failed to load keys from %s\n", keyfile);
        mpz_clears(p, q, n, phi, e, d, NULL);
        return -1;
    }

    int rc = encrypt(infile, outfile, e, n);
    if (rc != 0) fprintf(stderr, "encrypt() failed: %d\n", rc);

    mpz_clears(p, q, n, phi, e, d, NULL);
    return rc;
}

int handle_decrypt(int argc, char** argv) {
    char *keyfile = NULL, *infile = NULL, *outfile = NULL;
    if (parse_flags_common(argc, argv, &keyfile, &infile, &outfile) != 0) return -1;

    if (access(keyfile, R_OK) != 0) { fprintf(stderr, "Key file not found or unreadable: %s\n", keyfile); return -1; }
    if (access(infile, R_OK) != 0)  { fprintf(stderr, "Input file not found or unreadable: %s\n", infile); return -1; }

    if (strcmp(keyfile, infile) == 0 || strcmp(keyfile, outfile) == 0 || strcmp(infile, outfile) == 0) {
        fprintf(stderr, "Filenames must be distinct: --key, --in and --out must be different\n");
        return -1;
    }

    mpz_t p, q, n, e, phi, d;
    mpz_inits(p, q, n, phi, e, d, NULL);

    if (load_values(keyfile, p, q, n, phi, e, d) != 0) {
        fprintf(stderr, "Failed to load keys from %s\n", keyfile);
        mpz_clears(p, q, n, phi, e, d, NULL);
        return -1;
    }

    int rc = decrypt(infile, outfile, d, n);
    if (rc != 0) fprintf(stderr, "decrypt() failed: %d\n", rc);

    mpz_clears(p, q, n, phi, e, d, NULL);
    return rc;
}

int handle_dummy(int argc, char** argv) {
    (void)argc; (void)argv;
    fprintf(stderr, "Command not found\n");
    return -1;
}
