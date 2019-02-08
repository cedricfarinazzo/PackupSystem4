#ifndef _SRC_ENCRYPTION_AES_AES_ADDROUNDKEY_H_
#define _SRC_ENCRYPTION_AES_AES_ADDROUNDKEY_H_

#include <stdlib.h>

#include "aes_matrix.h"

struct AES_matrix *AES_matrix_addRoundKey(struct AES_matrix *block, struct AES_matrix *key);

#endif /* _SRC_ENCRYPTION_AES_AES_ADDROUNDKEY_H_ */
