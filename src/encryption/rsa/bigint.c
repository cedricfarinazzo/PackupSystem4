#include <stdlib.h>

#include "bigint.h"

struct RSA_bigint *RSA_bigint_new()
{
    struct RSA_bigint *a = malloc(sizeof(struct RSA_bigint));
    a->len = 0;
    a->neg = 0;
    a->capacity = 4;
    a->num = malloc(a->capacity * sizeof(int));
    return a;
}

void RSA_bigint_free(struct RSA_bigint *b)
{
    free(b->num);
    free(b);
}

void __RSA_bigint_double_capacity(struct RSA_bigint *b)
{
    if (b->len >= b->capacity)
    {
        b->capacity *= 2;
        b->num = realloc(b->num, b->capacity * sizeof(int));
    }
}

void RSA_bigint_longcpy(struct RSA_bigint *b, long i)
{
    if (i == 0)
    {
        b->len = 1;
        b->num[b->len - 1] = 0;
        return;
    }

    b->neg = i < 0;
    if (b->neg)
        i *= -1;
    b->len = 0;
    while (i != 0)
    {
        int a = (int)(i % 10);
        i /= 10;

        __RSA_bigint_double_capacity(b);
        b->num[b->len] = a;
        b->len++;
    }
}

void RSA_bigint_ulongcpy(struct RSA_bigint *b, unsigned long long i)
{
    if (i == 0)
    {
        b->len = 1;
        b->num[b->len - 1] = 0;
        return;
    }

    b->neg = 0;
    b->len = 0;
    while (i != 0)
    {
        int a = i % 10;
        i /= 10;

        __RSA_bigint_double_capacity(b);
        b->num[b->len] = a;
        b->len++;
    }
}

void RSA_bigint_ucharcpy(struct RSA_bigint *b, unsigned char *s)
{
    b->len = 0;
    if (s[0] == '\0')
        return;
    b->neg = 0;
    for(size_t i = 0; s[i] != '\0'; ++i)
    {
        __RSA_bigint_double_capacity(b);
        b->num[b->len] = (int)s[i];;
        b->len++;
    }
}

struct RSA_bigint *RSA_bigint_from_long(long i)
{
    struct RSA_bigint *a = RSA_bigint_new();
    RSA_bigint_longcpy(a, i);
    return a;
}

struct RSA_bigint *RSA_bigint_from_ulong(unsigned long long i)
{
    struct RSA_bigint *a = RSA_bigint_new();
    RSA_bigint_ulongcpy(a, i);
    return a;
}

struct RSA_bigint *RSA_bigint_from_uchar(unsigned char *s)
{
    struct RSA_bigint *a = RSA_bigint_new();
    RSA_bigint_ucharcpy(a, s);
    return a;
}


