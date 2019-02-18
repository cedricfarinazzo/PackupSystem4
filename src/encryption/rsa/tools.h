#ifndef _SRC_ENCRYPTION_RSA_TOOLS_H_
#define _SRC_ENCRYPTION_RSA_TOOLS_H_

#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#define ulong unsigned long long

void min(mpz_t a, mpz_t b, mpz_t r);

void max(mpz_t a, mpz_t b, mpz_t r);

mpz_t *ipow(mpz_t base, mpz_t exp);

mpz_t *square(mpz_t x);

int is_prime(mpz_t n);

ulong rand_a_b(ulong a, ulong b);

#endif /* _SRC_ENCRYPTION_RSA_RSA_H_ */
