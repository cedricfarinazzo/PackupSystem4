#include <stdlib.h>
#include "tools.h"
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

struct RSA_bigint *RSA_bigint_add(struct RSA_bigint *a, struct RSA_bigint *b)
{
    struct RSA_bigint *c = RSA_bigint_new();
    int ret = 0;

    size_t min_len = a->len < b->len ? a->len : b->len;
    size_t max_len = a->len > b->len ? a->len : b->len;

    for (size_t i = 0; i < min_len; ++i)
    {
        int sum = ret + a->num[i] + b->num[i];
        ret = sum / 10;
        sum = sum % 10;

        __RSA_bigint_double_capacity(c);
        c->num[c->len] = sum;
        c->len++;
    }

    for (size_t i = min_len; i < max_len; ++i)
    {
        if (i < a->len)
        {
            int sum = ret + a->num[i];
            ret = sum / 10;
            sum = sum % 10;
            
            __RSA_bigint_double_capacity(c);
            c->num[c->len] = sum;
            c->len++;
        }

        if (i < b->len)
        {
            int sum = ret + a->num[i];
            ret = sum / 10;
            sum = sum % 10;
            
            __RSA_bigint_double_capacity(c);
            c->num[c->len] = sum;
            c->len++;
        }
    }

    if (ret != 0)
    {

        __RSA_bigint_double_capacity(c);
        c->num[c->len] = ret;
        c->len++;
    }

    c->neg = a->neg || b->neg;

    return c;
}

struct RSA_bigint *RSA_bigint_add_long(struct RSA_bigint *a, long b)
{
    struct RSA_bigint *bi = RSA_bigint_from_long(b);
    struct RSA_bigint *c = RSA_bigint_add(a, bi);
    RSA_bigint_free(bi);
    return c;
}

struct RSA_bigint *RSA_bigint_add_ulong(struct RSA_bigint *a, ulong b)
{
    struct RSA_bigint *bi = RSA_bigint_from_ulong(b);
    struct RSA_bigint *c = RSA_bigint_add(a, bi);
    RSA_bigint_free(bi);
    return c;
}







