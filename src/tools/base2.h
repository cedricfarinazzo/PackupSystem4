#ifndef _SRC_ENCRYPTION_RSA_BASE2_H_
#define _SRC_ENCRYPTION_RSA_BASE2_H_

#include <stdlib.h>
#include <stdint.h>

char *base2_encode(char *in, size_t len, size_t *olen);

char *base2_decode(char *in, size_t len, size_t *olen);

#endif /* _SRC_ENCRYPTION_RSA_BASE2_H_ */
