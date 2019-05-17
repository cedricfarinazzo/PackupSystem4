#ifndef _SRC_ENCRYPTION_ELGAMAL_ELGAMAL_H_
#define _SRC_ENCRYPTION_ELGAMAL_ELGAMAL_H_

#include <stdlib.h>
#include "genkey.h"
#include "tools.h"


void EL_encryption_single(int c, long long *c1, long long *c2, struct ELGAMAL_pubkey *pub);

long long EL_decryption_single(long long c1, long long c2, struct ELGAMAL_privkey *priv);


#endif /* _SRC_ENCRYPTION_ELGAMAL_ELGAMAL_H_ */
