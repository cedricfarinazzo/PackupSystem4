#ifndef _SRC_ENCRYPTION_AES_AES_H_
#define _SRC_ENCRYPTION_AES_AES_H_

#include <stdlib.h>


size_t AES_encrypt(char *data, size_t data_size, char *key, size_t key_size, char **encrypt);

size_t AES_decrypt(char *encrypt, size_t encrypt_size, char *key, size_t key_size, char **decrypt);

#endif /* _SRC_ENCRYPTION_AES_AES_H_ */
