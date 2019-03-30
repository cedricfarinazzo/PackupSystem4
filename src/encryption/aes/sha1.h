#ifndef _SRC_ENCRYPTION_AES_SHA1_H_
#define _SRC_ENCRYPTION_AES_SHA1_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
    u_int32_t state[5];
    u_int32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;

#define SHA_DIGEST_LENGTH 512

void SHA1Transform(u_int32_t state[5], const unsigned char buffer[64]);

void SHA1Init(SHA1_CTX* context);

void SHA1Update(SHA1_CTX* context, const unsigned char* data, u_int32_t len);

void SHA1Final(unsigned char digest[20], SHA1_CTX* context);

#endif /* _SRC_ENCRYPTION_AES_SHA1_H_ */
