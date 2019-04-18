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

int RSA_encode_fd(int fin, int fout, struct RSA_pubKey *pubk);

int RSA_decode_fd(int fin, int fout, struct RSA_privKey *privk);

int RSA_encode_file(char *in, char *out, struct RSA_pubKey *pubk);

int RSA_decode_file(char *in, char *out, struct RSA_privKey *privk);

#endif /* _SRC_ENCRYPTION_RSA_RSA_FILE_H_ */
