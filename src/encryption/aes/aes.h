#ifndef _SRC_ENCRYPTION_AES_AES_H_
#define _SRC_ENCRYPTION_AES_AES_H_

#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_addroundkey.h"
#include "aes_subbytes.h"
#include "aes_shiftrows.h"
#include "aes_mixcolumns.h"

#define AES_OK 0
#define AES_ERROR_CANNOT_READ_FD -1
#define AES_ERROR_CANNOT_WRITE_FD -2
#define AES_ERROR_NULL_PATH -3
#define AES_ERROR_EMPTY_PATH -4
#define AES_ERROR_CANNOT_OPEN_FD -5

struct AES_ctx {
   struct AES_matrix *key;
   struct AES_matrix **roundKeys;
}AES_ctx;


/* AES_init: initialize aes with a key and return the context
 * key: unsigned char*: a 16 bit key
 * len: size_t: lenght of key
 * return: struct AES_ctx*: aes context
 */
struct AES_ctx *AES_init(unsigned char *key, size_t len);


/* AES_ctx_free: free an aes context
 * ctx: struct AES_ctx*: an aes context initilize by AES_init
 * return: void
 */
void AES_ctx_free(struct AES_ctx *ctx);


/* AES_encrypt: encrypt data with the context ctx and write the output on encrypt
 * ctx: struct AES_ctx*: an aes context initilize by AES_init
 * data: char*: data to encrypt
 * lendata: size_t: lenght of data
 * encrypt: char**: a pointer on char*. data will be malloc and write on *encrypt
 * return: size_t: lenght of encrypt
 */
size_t AES_encrypt(struct AES_ctx *ctx, unsigned char *data, size_t lendata, unsigned char **encrypt);


/* AES_decrypt: decrypt encrypted data with the context ctx and write the output on decrypt
 * ctx: struct AES_ctx*: an aes context initilize by AES_init
 * encrypt: char*: encrypt to decrypt
 * lendata: size_t: lenght of encrypt
 * decrypt: char**: a pointer on char*. data will be malloc and write on *decrypt
 * return: size_t: lenght of decrypt
 */
size_t AES_decrypt(struct AES_ctx *ctx, unsigned char *encrypt, size_t lendata, unsigned char **decrypt);

#endif /* _SRC_ENCRYPTION_AES_AES_H_ */
