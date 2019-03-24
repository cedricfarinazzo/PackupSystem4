#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashpass.h"

unsigned char *AES_keyFromPass(char *pass, size_t len)
{
    SHA_CTX ctx;
    SHA1_Init(&ctx);

    SHA1_Update(&ctx, pass, len);

    unsigned char tmphash[SHA_DIGEST_LENGTH];
    unsigned char hash[SHA_DIGEST_LENGTH*3];
    unsigned char *key = malloc(17 * sizeof(unsigned char));
    
    SHA1_Final(tmphash, &ctx);

    for (size_t i = 0; i < SHA_DIGEST_LENGTH; ++i)
        sprintf((char*)&(hash[i*2]), "%02x", tmphash[i]);
    
    for (size_t i = 0; i < 16; ++i)
        key[i] = hash[i];
    key[16] = 0;
    
    return key;
}
