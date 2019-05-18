#ifndef _SRC_ENCRYPTION_ELGAMAL_ELGAMAL_H_
#define _SRC_ENCRYPTION_ELGAMAL_ELGAMAL_H_

#include <stdlib.h>
#include "genkey.h"
#include "tools.h"


void EL_encryption_single(int c, long long *c1, long long *c2, struct ELGAMAL_pubkey *pub);

long long EL_decryption_single(long long c1, long long c2, struct ELGAMAL_privkey *priv);

void EL_encryption_stream(FILE *in, FILE *out, struct ELGAMAL_pubkey *pub);

void EL_decryption_stream(FILE *in, FILE *out, struct ELGAMAL_privkey *priv);

void EL_encryption_file(char *in, char *out, struct ELGAMAL_pubkey *pub);

void EL_decryption_file(char *in, char *out, struct ELGAMAL_privkey *priv);

void EL_encryption_fd(int in, int out, struct ELGAMAL_pubkey *pub);

void EL_decryption_fd(int in, int out, struct ELGAMAL_privkey *priv);

#endif /* _SRC_ENCRYPTION_ELGAMAL_ELGAMAL_H_ */
