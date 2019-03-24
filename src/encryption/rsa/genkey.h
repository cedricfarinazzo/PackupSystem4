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


/* RSA_generateKey: Generate private and public key
 * keysize: unsigned long: the number of bit of the key. must be a power of 2 (ex: 1024, 2048 or 4096)
 * privk: struct RSA_privKey*: a pointer on a struct RSA_privKey
 * pubk: struct RSA_pubKey*: a pointer on a struct RSA_pubKey
 * return: void
 */
void RSA_generateKey(unsigned long keysize, struct RSA_privKey **privk, struct RSA_pubKey **pubk);


/* RSA_free_private_key: free the private key
 * privk: struct RSA_privKey*: a private key generated RSA_generateKey
 * return: void
 */
void RSA_free_private_key(struct RSA_privKey *privk);


/* RSA_free_public_key: free the public key
 * pubk: struct RSA_pubKey*: a public key generated RSA_generateKey
 * return: void
 */
void RSA_free_public_key(struct RSA_pubKey *pubk);

#endif /* _SRC_ENCRYPTION_RSA_GENKEY_H_ */
