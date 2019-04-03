#ifndef _SRC_ENCRYPTION_AES_AES_FILE_H_
#define _SRC_ENCRYPTION_AES_AES_FILE_H_

#include <stdlib.h>
#include <unistd.h>

#include "aes.h"
#include "hashpass.h"

#define BUFFER_SIZE 512

/* AES_encrypt_file: encrypt data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: char*: a password
 * return: int: 0 on success, -1 on failure
 */
int AES_encrypt_file(int fin, int fout, char *pass);


/* AES_decrypt_file: decrypt data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: char*: a password
 * return: int: 0 on success, -1 on failure
 */
int AES_decrypt_file(int fin, int fout, char *pass);

#endif /* _SRC_ENCRYPTION_AES_AES_FILE_H__ */
