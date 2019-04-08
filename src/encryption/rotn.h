#ifndef _SRC_ENCRYPTION_ROTN_H_
#define _SRC_ENCRYPTION_ROTN_H_

#include <stddef.h>

#define BUFFER_SIZE 512

void ROTN_encrypt(char *data, int key);

void ROTN_decrypt(char *data, int key);

int ROTN_encrypt_file(int fin, int fout, int key);

int ROTN_decrypt_file(int fin, int fout, int key);

#endif /* _SRC_ENCRYPTION_ROTN_H_ */
