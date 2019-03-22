#ifndef _SRC_ENCRYPTION_AES_HASSPASS_H__
#define _SRC_ENCRYPTION_AES_HASSPASS_H_

#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char *AES_keyFromPass(char *pass, size_t len);

#endif /* _SRC_ENCRYPTION_AES_HASSPASS_H__ */
