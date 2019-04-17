#ifndef _SRC_ENCRYPTION_ROTN_H_
#define _SRC_ENCRYPTION_ROTN_H_

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 512

#define ROTN_OK 0
#define ROTN_ERROR_CANNOT_READ_FD -1
#define ROTN_ERROR_CANNOT_WRITE_FD -2
#define ROTN_ERROR_NULL_PATH -3
#define ROTN_ERROR_EMPTY_PATH -4
#define ROTN_ERROR_CANNOT_OPEN_FD -5

void ROTN_encrypt(char *data, int key);

void ROTN_decrypt(char *data, int key);

int ROTN_encrypt_fd(int fin, int fout, int key);

int ROTN_decrypt_fd(int fin, int fout, int key);

int ROTN_encrypt_file(char *in, char *out, int key);

int ROTN_decrypt_file(char *in, char *out, int key);

#endif /* _SRC_ENCRYPTION_ROTN_H_ */
