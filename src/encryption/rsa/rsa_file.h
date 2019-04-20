#ifndef _SRC_ENCRYPTION_RSA_RSA_FILE_H_
#define _SRC_ENCRYPTION_RSA_RSA_FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "genkey.h"
#include "rsa.h"

#define RSA_BUFFER_SIZE_E 128
#define RSA_BUFFER_SIZE_D 258

/* RSA_encode_fd: encode data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: struct RSA_pubKey*: RSA public key
 * return: int: RSA code (check tools.h)
 */
int RSA_encode_fd(int fin, int fout, struct RSA_pubKey *pubk);


/* RSA_decode_fd: decode data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: struct RSA_privKey*: RSA private key
 * return: int: RSA code (check tools.h)
 */
int RSA_decode_fd(int fin, int fout, struct RSA_privKey *privk);


/* RSA_encode_file: encode data from in file and write on out file
 * fin: char*: path to the input file
 * fout: int: path to the output file
 * pass: struct RSA_pubKey*: RSA public key
 * return: int: RSA code (check tools.h)
 */
int RSA_encode_file(char *in, char *out, struct RSA_pubKey *pubk);


/* RSA_decode_file: decode data from in file and write on out file
 * fin: char*: path to the input file
 * fout: int: path to the output file
 * pass: struct RSA_privKey*: RSA private key
 * return: int: RSA code (check tools.h)
 */
int RSA_decode_file(char *in, char *out, struct RSA_privKey *privk);

#endif /* _SRC_ENCRYPTION_RSA_RSA_FILE_H_ */
