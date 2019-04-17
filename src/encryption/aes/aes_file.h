#ifndef _SRC_ENCRYPTION_AES_AES_FILE_H_
#define _SRC_ENCRYPTION_AES_AES_FILE_H_

#include <stdlib.h>
#include <unistd.h>

#include "aes.h"
#include "hashpass.h"

#define AES_BUFFER_SIZE 16

#define AES_OK 0
#define AES_ERROR_CANNOT_READ_FD -1
#define AES_ERROR_CANNOT_WRITE_FD -2
#define AES_ERROR_NULL_PATH -3
#define AES_ERROR_EMPTY_PATH -4
#define AES_ERROR_CANNOT_OPEN_FD -5

/* AES_encrypt_fd: encrypt data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: char*: a password
 * return: int: AES code (check aes_file.h)
 */
int AES_encrypt_fd(int fin, int fout, char *pass);


/* AES_decrypt_fd: decrypt data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: char*: a password
 * return: int: AES code (check aes_file.h)
 */
int AES_decrypt_fd(int fin, int fout, char *pass);

/* AES_encrypt_file: encrypt data from in and write on out
 * in: char*: path to the input file
 * out: char*: path to the output file
 * pass: char*: a password
 * return: int: AES code (check aes_file.h)
 */
int AES_encrypt_file(char *in, char *out, char *pass);


/* AES_decrypt_fd: decrypt data from in and write on out
 * fin: char*: path to the input file
 * fout: char*: path to the output file
 * pass: char*: a password
 * return: int: AES code (check aes_file.h)
 */
int AES_decrypt_file(char *in, char *out, char *pass);
#endif /* _SRC_ENCRYPTION_AES_AES_FILE_H__ */
