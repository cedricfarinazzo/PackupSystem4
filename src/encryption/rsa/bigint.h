#ifndef _SRC_ENCRYPTION_RSA_BIGINT_H_
#define _SRC_ENCRYPTION_RSA_BIGINT_H_

#include <stdlib.h>
#include "tools.h"

struct RSA_bigint {
    size_t capacity;
    size_t len;
    int neg;
    int *num;
} RSA_bigint;

#endif /* _SRC_ENCRYPTION_RSA_BIGINT_H_ */
