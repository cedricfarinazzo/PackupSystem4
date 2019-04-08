#ifndef _SRC_ENCRYPTION_VIGENERE_H_
#define _SRC_ENCRYPTION_VIGENERE_H_

#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

void VIGENERE_encrypt(char *data, char *key);

void VIGENERE_decrypt(char *data, char *key);

#endif /* _SRC_ENCRYPTION_VIGENERE_H_ */
