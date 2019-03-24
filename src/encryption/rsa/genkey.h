#ifndef _SRC_ENCRYPTION_RSA_GENKEY_H_
#define _SRC_ENCRYPTION_RSA_GENKEY_H_

#include <stdlib.h>
#include <gmp.h>
#include "tools.h"

struct RSA_pubKey {
    mpz_t *n;
    mpz_t *e;
} RSA_publickey;

struct RSA_privKey {
    mpz_t *n;
    mpz_t *d;
} RSA_privatekey;


void RSA_generateKey(unsigned long keysize, struct RSA_privKey **privk, struct RSA_pubKey **pubk);

void RSA_free_private_key(struct RSA_privKey *privk);

void RSA_free_public_key(struct RSA_pubKey *pubk);

#endif /* _SRC_ENCRYPTION_RSA_GENKEY_H_ */
