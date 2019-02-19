#ifndef _SRC_ENCRYPTION_RSA_GENKEY_H_
#define _SRC_ENCRYPTION_RSA_GENKEY_H_

#include <stdlib.h>
#include <gmp.h>
#include "tools.h"

struct RSA_publickey {
    mpz_t *n;
    mpz_t *e;
} RSA_publickey;

struct RSA_privatekey {
    mpz_t *n;
    mpz_t *d;
} RSA_privatekey;

struct RSA_publickey *RSA_gen_public_key(mpz_t p, mpz_t q);

void RSA_free_public_key(struct RSA_publickey *pub);

struct RSA_privatekey *RSA_gen_private_key(mpz_t p, mpz_t q, struct RSA_publickey *public);

void RSA_free_private_key(struct RSA_privatekey *priv);

#endif /* _SRC_ENCRYPTION_RSA_GENKEY_H_ */
