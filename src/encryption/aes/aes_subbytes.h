#ifndef _SRC_ENCRYPTION_AES_AES_SUBBYTES_H_
#define _SRC_ENCRYPTION_AES_AES_SUBBYTES_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "aes_matrix.h"

struct AES_matrix *AES_matrix_subBytes(struct AES_matrix *block);

struct AES_matrix *AES_matrix_InvSubBytes(struct AES_matrix *block);

int AES_matrix_subBytesInt(int v);

#endif /* _SRC_ENCRYPTION_AES_AES_SUBBYTES_H_ */
