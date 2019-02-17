#ifndef _SRC_ENCRYPTION_RSA_GENKEY_H_
#define _SRC_ENCRYPTION_RSA_GENKEY_H_

#include <stdlib.h>
#include "tools.h"

ulong *RSA_gen_public_key(ulong p, ulong q);

ulong *RSA_gen_private_key(ulong p, ulong q, ulong *public);

#endif /* _SRC_ENCRYPTION_RSA_GENKEY_H_ */
