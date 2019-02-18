#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include "tools.h"

void min(mpz_t a, mpz_t b, mpz_t r)
{
    if (mpz_cmp(a, b) < 0)
    {
        mpz_set(r,a);
    } else { 
        mpz_set(r, b);
    }
}

void max(mpz_t a, mpz_t b, mpz_t r)
{
    if (mpz_cmp(a, b) > 0)
    {
        mpz_set(r,a);
    } else { 
        mpz_set(r, b);
    }
}

mpz_t *ipow(mpz_t base, mpz_t exp)
{
    mpz_t e;
    mpz_set(e, exp);
    mpz_t *r = malloc(sizeof(mpz_t));
    mpz_init(*r);
    mpz_set_ui(*r,1);
    for (;mpz_sgn(e) > 0; mpz_sub_ui(e, e, 1))
        mpz_mul(*r, *r, base);
    mpz_clear(e);
    return r;
}

mpz_t *square(mpz_t x)
{ 
    mpz_t r;
    mpz_init(r);
    mpz_set_ui(r,2);
    mpz_t *a = ipow(x, r);
    mpz_clear(r);
    return a;
}

int is_prime(mpz_t n)
{
    return mpz_probab_prime_p(n, 50) == 2;
}

ulong rand_a_b(ulong a, ulong b)
{
    return rand() % (b - a) + a;
}
