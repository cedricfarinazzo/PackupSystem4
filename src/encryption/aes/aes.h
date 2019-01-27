#ifndef _SRC_ENCRYPTION_AES_AES_H_
#define _SRC_ENCRYPTION_AES_AES_H_

#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_addroundkey.h"
#include "aes_subbytes.h"
#include "aes_shiftrows.h"
#include "aes_mixcolumns.h"

void AES_encrypt(char *data, char *key, char **encrypt);

void AES_decrypt(char *encrypt, char *key, char **decrypt);

#endif /* _SRC_ENCRYPTION_AES_AES_H_ */
