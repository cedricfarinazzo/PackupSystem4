#ifndef _SRC_ENCRYPTION_RSA_RSA_H_
#define _SRC_ENCRYPTION_RSA_RSA_H_

#include <stdlib.h>
#include <gmp.h>
#include "genkey.h"
#include "tools.h"

void single_encode_rsa(struct RSA_pubKey *public, mpz_t c, mpz_t r);

void single_decode_rsa(struct RSA_privKey *private, mpz_t c, mpz_t r);

/* RSA_encode: encrypt a char* with a public key
 * public: struct RSA_pubKey*: a public key generated RSA_generateKey
 * data: unsigned char*: message to encrypt
 * len: size_t: lenght of data
 * rlen: size_t: lenght of output (mpz_t *)
 * return: mpz_t*: an array of mpz_t (lenght = rlen). it's the encrypted message
 */
unsigned char *RSA_encode(struct RSA_pubKey *public, unsigned char *data, size_t len, size_t *rlen) __attribute__((deprecated));


/* RSA_decode: decrypt a char* with a public key
 * private: struct RSA_privKey*: a private key generated RSA_generateKey
 * data: mpz_t*: message to decrypt
 * len: size_t: lenght of data
 * return: unsigned char*:(lenght = len). it's the decrypted message
 */
unsigned char *RSA_decode(struct RSA_privKey *private, unsigned char *data, size_t len, size_t *rlen) __attribute__((deprecated));

#endif /* _SRC_ENCRYPTION_RSA_RSA_H_ */
