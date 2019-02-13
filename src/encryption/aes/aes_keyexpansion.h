#ifndef _SRC_ENCRYPTION_AES_AES_KEYEXPANSION_H_
#define _SRC_ENCRYPTION_AES_AES_KEYEXPANSION_H_

#include "aes_matrix.h"

#define AES_NB_ROUND_KEY 10

struct AES_matrix **AES_keyExpansion(struct AES_matrix *key);

void AES_keyExpansion_free(struct AES_matrix **roundKey);

#endif /* _SRC_ENCRYPTION_AES_AES_KEYEXPANSION_H_ */
