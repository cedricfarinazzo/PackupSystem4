#ifndef _SRC_ENCRYPTION_AES_AES_FILE_H_
#define _SRC_ENCRYPTION_AES_AES_FILE_H_

#include <stdlib.h>
#include <unistd.h>

#include "aes.h"
#include "hashpass.h"

#define BUFFER_SIZE 512

int AES_encrypt_file(int fin, int fout, char *pass);

int AES_decrypt_file(int fin, int fout, char *pass);

#endif /* _SRC_ENCRYPTION_AES_AES_FILE_H__ */
