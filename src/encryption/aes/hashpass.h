#ifndef _SRC_ENCRYPTION_AES_HASSPASS_H_
#define _SRC_ENCRYPTION_AES_HASSPASS_H_

#include <stdio.h>
#include <stdlib.h>

#include "sha1.h"

/* AES_keyFromPass: generate an aes key from password
 * pass: char*: a password
 * len: size_t: lenght of pass
 * return: unsigned char*: an aes key of 16 byte ended by null character
 */
unsigned char *AES_keyFromPass(char *pass, size_t len);

#endif /* _SRC_ENCRYPTION_AES_HASSPASS_H_ */
