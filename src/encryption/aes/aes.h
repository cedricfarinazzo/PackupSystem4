#ifndef _SRC_ENCRYPTION_AES_AES_H_
#define _SRC_ENCRYPTION_AES_AES_H_

#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_addroundkey.h"
#include "aes_subbytes.h"
#include "aes_shiftrows.h"
#include "aes_mixcolumns.h"

struct AES_ctx {
   struct AES_matrix *key;
   struct AES_matrix **roundKeys;
}AES_ctx;

struct AES_ctx *AES_init(unsigned char *key, size_t len);

void AES_ctx_free(struct AES_ctx *ctx);

size_t AES_encrypt(struct AES_ctx *ctx, unsigned char *data, size_t lendata, unsigned char **encrypt);

size_t AES_decrypt(struct AES_ctx *ctx, unsigned char *encrypt, size_t lendata, unsigned char **decrypt);

#endif /* _SRC_ENCRYPTION_AES_AES_H_ */
