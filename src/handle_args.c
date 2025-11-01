#include "handle_args.h"

const size_t num_commands = 3;
const char* commands[] = {"genkey", "encrypt", "decrypt"};
int (*handlers[])(int, char **) = {handle_genkey, handle_encrypt, handle_decrypt, handle_dummy};

int find_command(char* command) {
    for(size_t i = 0; i < num_commands; ++i)
        if (strcmp(command, commands[i]) == 0) return (int)i;
    return (int)num_commands;
}

/* Centralized parser.
   - mode = 0 : genkey (only --key optional)
   - mode != 0 : encrypt/decrypt (requires --in, optional --key/--out)
   Returns 0 on ok, -1 on error.
*/
static int parse_input(int argc, char **argv, int mode, cmd_opts_t *out) {
    static struct option longopts[] = {
        {"key", required_argument, NULL, 'k'},
        {"in",  required_argument, NULL, 'i'},
        {"out", required_argument, NULL, 'o'},
        {0,0,0,0}
    };

    out->keyfile = "files/keys.txt";
    out->outfile = "files/crypt_out.txt";
    out->infile = NULL;

    optind = 2; /* skip program + command */
    int c;
    while ((c = getopt_long(argc, argv, "k:i:o:", longopts, NULL)) != -1) {
        switch (c) {
            case 'k': out->keyfile = optarg; break;
            case 'i': out->infile  = optarg; break;
            case 'o': out->outfile = optarg; break;
            case '?':
            default:
                return -1;
        }
    }

    if (mode != 0 && !out->infile) {
        fprintf(stderr, "Missing required --in <file>\n");
        return -1;
    }
    return 0;
}

static int check_filenames(const cmd_opts_t *o) {
    if (!o) return -1;
    if (!o->infile) return 0; /* if no infile (genkey) it's fine */
    if (strcmp(o->keyfile, o->infile) == 0 ||
        strcmp(o->keyfile, o->outfile) == 0 ||
        strcmp(o->infile, o->outfile) == 0) {
        fprintf(stderr, "Filenames must be distinct (--key, --in, --out)\n");
        return -1;
    }
    return 0;
}

// ---------------------------- handlers 

int handle_genkey(int argc, char** argv) {
    cmd_opts_t opts;
    if (parse_input(argc, argv, 0, &opts) != 0) {
        fprintf(stderr, "Usage: %s genkey [--key <file>]\n", argv[0]);
        return -1;
    }

    mpz_t p,q,n,phi,e,d;
    mpz_inits(p,q,n,phi,e,d, NULL);

    gen_keys(p,q,n,phi,e,d);
    if (save_keys(opts.keyfile, n, e, d) != 0) {
        fprintf(stderr, "Failed saving keys to %s\n", opts.keyfile);
        mpz_clears(p,q,n,phi,e,d, NULL);
        return -1;
    }

    printf("Keys saved to %s\n", opts.keyfile);
    mpz_clears(p,q,n,phi,e,d, NULL);
    return 0;
}

int handle_encrypt(int argc, char** argv) {
    cmd_opts_t opts;
    if (parse_input(argc, argv, 1, &opts) != 0) {
        fprintf(stderr, "Usage: %s encrypt --in <file> [--key <file>] [--out <file>]\n", argv[0]);
        return -1;
    }
    if (check_filenames(&opts) != 0) return -1;
    if (access(opts.keyfile, R_OK) != 0) { fprintf(stderr, "Key file not readable: %s\n", opts.keyfile); return -1; }
    if (access(opts.infile, R_OK) != 0) { fprintf(stderr, "Input file not readable: %s\n", opts.infile); return -1; }

    mpz_t n,e,d;
    mpz_inits(n,e,d, NULL);
    if (load_keys(opts.keyfile, n, e, d) != 0) {
        fprintf(stderr, "Failed loading keys from %s\n", opts.keyfile);
        mpz_clears(n,e,d, NULL);
        return -1;
    }

    int rc = encrypt(opts.infile, opts.outfile, e, n);
    if (rc != 0) fprintf(stderr, "encrypt() failed: %d\n", rc);
    mpz_clears(n,e,d, NULL);
    return rc;
}

int handle_decrypt(int argc, char** argv) {
    cmd_opts_t opts;
    if (parse_input(argc, argv, 1, &opts) != 0) {
        fprintf(stderr, "Usage: %s decrypt --in <file> [--key <file>] [--out <file>]\n", argv[0]);
        return -1;
    }
    if (check_filenames(&opts) != 0) return -1;
    if (access(opts.keyfile, R_OK) != 0) { fprintf(stderr, "Key file not readable: %s\n", opts.keyfile); return -1; }
    if (access(opts.infile, R_OK) != 0) { fprintf(stderr, "Input file not readable: %s\n", opts.infile); return -1; }

    mpz_t n,e,d;
    mpz_inits(n,e,d, NULL);
    if (load_keys(opts.keyfile, n, e, d) != 0) {
        fprintf(stderr, "Failed loading keys from %s\n", opts.keyfile);
        mpz_clears(n,e,d, NULL);
        return -1;
    }

    int rc = decrypt(opts.infile, opts.outfile, d, n);
    if (rc != 0) fprintf(stderr, "decrypt() failed: %d\n", rc);
    mpz_clears(n,e,d, NULL);
    return rc;
}

int handle_dummy(int argc, char** argv) {
    (void)argc; (void)argv;
    fprintf(stderr, "Command not found\n");
    return -1;
}
