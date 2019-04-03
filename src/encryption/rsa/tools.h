#ifndef _SRC_ENCRYPTION_RSA_TOOLS_H_
#define _SRC_ENCRYPTION_RSA_TOOLS_H_

#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#define ulong unsigned long long

void min(mpz_t a, mpz_t b, mpz_t r);

void max(mpz_t a, mpz_t b, mpz_t r);

void ipow(mpz_t base, mpz_t exp, mpz_t r);

void square(mpz_t x, mpz_t a);

int is_prime(mpz_t n);

ulong rand_a_b(ulong a, ulong b);

unsigned char *get_str(unsigned long base, mpz_t x, size_t *len);

#endif /* _SRC_ENCRYPTION_RSA_RSA_H_ */
