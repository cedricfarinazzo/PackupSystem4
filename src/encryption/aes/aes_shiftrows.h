#ifndef _SRC_ENCRYPTION_AES_AES_SHIFTROWS_H_
#define _SRC_ENCRYPTION_AES_AES_SHIFTROWS_H_

#include <stdlib.h>

#include "aes_matrix.h"

struct AES_matrix *AES_matrix_shiftRows(struct AES_matrix *block);

struct AES_matrix *AES_matrix_InvShiftRows(struct AES_matrix *block);

#endif /* _SRC_ENCRYPTION_AES_AES_SHIFTROWS_H_ */
