#ifndef _SRC_ENCRYPTION_RSA_RSA_H_
#define _SRC_ENCRYPTION_RSA_RSA_H_

#include <stdlib.h>
#include <gmp.h>
#include "genkey.h"
#include "tools.h"


/* RSA_encode: encrypt a char* with a public key
 * public: struct RSA_pubKey*: a public key generated RSA_generateKey
 * data: unsigned char*: message to encrypt
 * len: size_t: lenght of data
 * return: mpz_t*: an array of mpz_t (lenght = len). it's the encrypted message
 */
mpz_t *RSA_encode(struct RSA_pubKey *public, unsigned char *data, size_t len);


/* RSA_decode: decrypt a char* with a public key
 * private: struct RSA_privKey*: a private key generated RSA_generateKey
 * data: mpz_t*: message to decrypt
 * len: size_t: lenght of data
 * return: unsigned char*:(lenght = len). it's the decrypted message
 */
unsigned char *RSA_decode(struct RSA_privKey *private, mpz_t *data, size_t len);

#endif /* _SRC_ENCRYPTION_RSA_RSA_H_ */
