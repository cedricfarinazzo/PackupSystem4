#ifndef _SRC_ENCRYPTION_RSA_RSA_H_
#define _SRC_ENCRYPTION_RSA_RSA_H_

#include <stdlib.h>
#include "genkey.h"
#include "tools.h"

ulong *RSA_encode(ulong *public, char *data, size_t len);

unsigned char *RSA_decode(ulong *private, ulong *data, size_t len);

#endif /* _SRC_ENCRYPTION_RSA_RSA_H_ */
