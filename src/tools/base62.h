#ifndef _SRC_ENCRYPTION_RSA_BASE62_H_
#define _SRC_ENCRYPTION_RSA_BASE62_H_

#include <stdlib.h>
#include <stdint.h>

char *base62_encode(char *in, size_t len, size_t *olen);

char *base62_decode(char *in, size_t len, size_t *olen);

#endif /* _SRC_ENCRYPTION_RSA_BASE62_H_ */
