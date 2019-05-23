#ifndef _SRC_ENCRYPTION_RSA_TOOLS_H_
#define _SRC_ENCRYPTION_RSA_TOOLS_H_

#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#define RSA_OK 0
#define RSA_ERROR_CANNOT_READ_FD -1
#define RSA_ERROR_CANNOT_WRITE_FD -2
#define RSA_ERROR_NULL_PATH -3
#define RSA_ERROR_EMPTY_PATH -4
#define RSA_ERROR_CANNOT_OPEN_FD -5

#define ulong unsigned long long

void min(mpz_t a, mpz_t b, mpz_t r);

void max(mpz_t a, mpz_t b, mpz_t r);

void ipow(mpz_t base, mpz_t exp, mpz_t r);

void square(mpz_t x, mpz_t a);

int is_prime(mpz_t n);

ulong rand_a_b(ulong a, ulong b);

#endif /* _SRC_ENCRYPTION_RSA_RSA_H_ */
