#ifndef _SRC_ENCRYPTION_ENCRYPTION_H_
#define _SRC_ENCRYPTION_ENCRYPTION_H_

#include <stdlib.h>
#include <stdio.h>

#include "rotn.h"
#include "tea.h"
#include "vigenere.h"
#include "rsa/rsa.h"
#include "rsa/rsa_file.h"
#include "rsa/genkey.h"
#include "aes/aes.h"
#include "aes/aes_file.h"
#include "aes/hashpass.h"
#include "elgamal/elgamal.h"

int file_exist(char *fname);

enum ENCRYPTION_TYPE {
    ROTN,
    VIGENERE,
    RSA, 
    AES,
    ELGAMAL,
    NONE
};

/*
 * int PACKUP_encryption_stream(enum ENCRYPTION_TYPE type, FILE *in, FILE *out, ...)
 * ARG: 
 *    enum ENCRYPTION_TYPE type: encryption algorithm selection
 *    FILE *in: stream on input file
 *    FILE *out stream on output file
                   |                                 ARGS                                   |
    Algorithm      |     1     |     2     |     3     |     4      |     5     |     6     |
  -------------------------------------------------------------------------------------------
  No encryption    |    NONE   |    NULL   |    NULL   |     X      |     X     |     X     |
  -------------------------------------------------------------------------------------------
  Rotn encryption  |    ROTN   |  FILE *in | FILE *out |  int key   |     X     |     X     |
  -------------------------------------------------------------------------------------------
  Vigenere enc     | VIGENERE  |  FILE *in | FILE *out | char *key  |     X     |     X     |
  -------------------------------------------------------------------------------------------
  AES enc          |    AES    |  FILE *in | FILE *out | char *pass |     X     |     X     |
  -------------------------------------------------------------------------------------------
  RSA enc          |    RSA    |  FILE *in | FILE *out | char *pub  |     X     |     X     |
   (use of an      |           |           |           | (path to   |           |           |
   existing public |           |           |           | public key)|           |           |
   key file)       |           |           |           |            |           |           |
  -------------------------------------------------------------------------------------------
  RSA enc          |    RSA    |  FILE *in | FILE *out | char *pub  | char *priv|unsigned   |
   (creation       |           |           |           | (path to   | (path to  | long size |
   of keys)        |           |           |           | public key)|private key| (key size)|
  -------------------------------------------------------------------------------------------
  ELGAMAL enc      | ELGAMAL   |  FILE *in | FILE *out | char *pub  |     X     |     X     |
   (use of an      |           |           |           | (path to   |           |           |
   existing public |           |           |           | public key)|           |           |
   key file)       |           |           |           |            |           |           |
  -------------------------------------------------------------------------------------------
  ELGAMAL enc      | ELGAMAL   |  FILE *in | FILE *out | char *pub  | char *priv|unsigned   |
   (creation       |           |           |           | (path to   | (path to  | long size |
   of keys)        |           |           |           | public key)|private key| (key size)|
  -------------------------------------------------------------------------------------------
 * RETURN TYPE: 0 if done otherwise an integer
*/
int PACKUP_encryption_stream(enum ENCRYPTION_TYPE type, FILE *in, FILE *out, ...);


/*
 * int PACKUP_decryption_stream(enum ENCRYPTION_TYPE type, FILE *in, FILE *out, ...)
 * ARG: 
 *    enum ENCRYPTION_TYPE type: decryption algorithm selection
 *    FILE *in: stream on input file
 *    FILE *out stream on output file
                   |                                 ARGS                                   |
    Algorithm      |     1     |     2     |     3     |     4      |     5     |     6     |
  -------------------------------------------------------------------------------------------
  No decryption    |    NONE   |    NULL   |    NULL   |     X      |     X     |     X     |
  -------------------------------------------------------------------------------------------
  Rotn decryption  |    ROTN   |  FILE *in | FILE *out |  int key   |     X     |     X     |
  -------------------------------------------------------------------------------------------
  Vigenere dec     | VIGENERE  |  FILE *in | FILE *out | char *key  |     X     |     X     |
  -------------------------------------------------------------------------------------------
  AES dec          |    AES    |  FILE *in | FILE *out | char *pass |     X     |     X     |
  -------------------------------------------------------------------------------------------
  RSA dec          |    RSA    |  FILE *in | FILE *out | char *priv |     X     |     X     |
   (use of an      |           |           |           | (path to   |           |           |
   existing private|           |           |           |private key)|           |           |
   key file)       |           |           |           |            |           |           |
  -------------------------------------------------------------------------------------------
  ELGAMAL enc      | ELGAMAL   |  FILE *in | FILE *out | char *priv |     X     |     X     |
   (use of an      |           |           |           | (path to   |           |           |
   existing private|           |           |           |private key)|           |           |
   key file)       |           |           |           |            |           |           |
  -------------------------------------------------------------------------------------------
 * RETURN TYPE: 0 if done otherwise an integer
*/
int PACKUP_decryption_stream(enum ENCRYPTION_TYPE type, FILE *in, FILE *out, ...);


#endif /* _SRC_ENCRYPTION_ENCRYPTION_H_ */
