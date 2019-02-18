#ifndef _SRC_ENCRYPTION_RSA_TOOLS_H_
#define _SRC_ENCRYPTION_RSA_TOOLS_H_

#include <stdlib.h>
#include <time.h>

#define ulong unsigned long long

ulong min(ulong a, ulong b);

ulong max(ulong a, ulong b);

ulong ipow(ulong base, ulong exp);

ulong square(ulong x);

int is_prime(ulong n);

ulong pgcd(ulong a, ulong b);

#endif /* _SRC_ENCRYPTION_RSA_RSA_H_ */
