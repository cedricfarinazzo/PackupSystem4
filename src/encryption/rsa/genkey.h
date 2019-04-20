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


/* RSA_pubk_to_file: write the RSA public to file
 * pub: struct RSA_pubKey*: RSA public key
 * path: int: path to the output file
 * return: int: RSA code (check tools.h)
 */
int RSA_pubk_to_file(struct RSA_pubKey *pub, char *path);


/* RSA_privk_to_file: write the RSA private to file
 * pub: struct RSA_privKey*: RSA private key
 * path: int: path to the output file
 * return: int: RSA code (check tools.h)
 */
int RSA_privk_to_file(struct RSA_privKey *priv, char *path);


/* RSA_pubk_from_file: load a RSA public from file
 * path: int: path to file
 * return: struct RSA_pubKey*: RSA public key
 */
struct RSA_pubKey *RSA_pubKey_from_file(char *path);


/* RSA_privKey_from_file: load a RSA private from file
 * path: int: path to file
 * return: struct RSA_privKey*: RSA private key
 */
struct RSA_privKey *RSA_privKey_from_file(char *path);

#endif /* _SRC_ENCRYPTION_RSA_GENKEY_H_ */
