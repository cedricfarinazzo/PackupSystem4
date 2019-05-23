#ifndef _SRC_ENCRYPTION_VIGENERE_H_
#define _SRC_ENCRYPTION_VIGENERE_H_

#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define VIGENERE_BUFFER_SIZE 512

#define VIGENERE_OK 0
#define VIGENERE_ERROR_CANNOT_READ_FD -1
#define VIGENERE_ERROR_CANNOT_WRITE_FD -2
#define VIGENERE_ERROR_NULL_PATH -3
#define VIGENERE_ERROR_EMPTY_PATH -4
#define VIGENERE_ERROR_CANNOT_OPEN_FD -5

void VIGENERE_encrypt(char *data, char *key);

void VIGENERE_decrypt(char *data, char *key);

int VIGENERE_encrypt_fd(int fin, int fout, char *key);

int VIGENERE_decrypt_fd(int fin, int fout, char *key);

int VIGENERE_encrypt_stream(FILE *fin, FILE *fout, char *key);

int VIGENERE_decrypt_stream(FILE *fin, FILE *fout, char *key);

int VIGENERE_encrypt_file(char *in, char *out, char *key);

int VIGENERE_decrypt_file(char *in, char *out, char *key);

#endif /* _SRC_ENCRYPTION_VIGENERE_H_ */
