#ifndef _SRC_ENCRYPTION_RSA_RSA_H_
#define _SRC_ENCRYPTION_RSA_RSA_H_

#include <stdlib.h>
#include <gmp.h>
#include "genkey.h"
#include "tools.h"

mpz_t *RSA_encode(struct RSA_publickey *public, unsigned char *data, size_t len);

unsigned char *RSA_decode(struct RSA_privatekey *private, mpz_t *data, size_t len);

#endif /* _SRC_ENCRYPTION_RSA_RSA_H_ */
