#ifndef _SRC_ENCRYPTION_ELGAMAL_GENKEY_H_
#define _SRC_ENCRYPTION_ELGAMAL_GENKEY_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "tools.h"

#define EL_OK 0
#define EL_ERROR_CANNOT_READ_FD -1
#define EL_ERROR_CANNOT_WRITE_FD -2
#define EL_ERROR_NULL_PATH -3
#define EL_ERROR_EMPTY_PATH -4
#define EL_ERROR_CANNOT_OPEN_FD -5

struct ELGAMAL_privkey {
    long long p, d;
} ELGAMAL_privkey;

struct ELGAMAL_pubkey {
    long long p, e1, e2;
} ELGAMAL_pubkey;


void ELGAMAL_generateKey(size_t size, struct ELGAMAL_privkey **privk, struct ELGAMAL_pubkey **pubk);


void ELGAMAL_privkey_free(struct ELGAMAL_privkey *k);


void ELGAMAL_pubkey_free(struct ELGAMAL_pubkey *k);


int ELGAMAL_pubk_to_file(struct ELGAMAL_pubkey *pub, char *path);


int ELGAMAL_privk_to_file(struct ELGAMAL_privkey *priv, char *path);


struct ELGAMAL_pubkey *ELGAMAL_pubKey_from_file(char *path);


struct ELGAMAL_privkey *ELGAMAL_privKey_from_file(char *path);

#endif /* _SRC_ENCRYPTION_ELGAMAL_GENKEY_H_ */
