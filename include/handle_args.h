#ifndef HANDLE_ARGS
#define HANDLE_ARGS

#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include "crypt.h"
#include "store.h"
#include "gen_keys.h"

#define COMMAND   1

int handle_genkey (int, char**);
int handle_encrypt(int, char**);
int handle_decrypt(int, char**);
int handle_dummy  (int, char**);

int find_command(char* command);

extern const size_t num_commands;
extern const char* commands[];
extern const char* options[];
extern int (*handlers[])(int, char **);

typedef struct {
    const char *keyfile;
    const char *infile;
    const char *outfile;
} cmd_opts_t;

#endif