#include <stdlib.h>
#include "genKey.h"

#define ulong unsigned long

ulong min(ulong a, ulong b)
{
    return a <= b ? a : b;
}

ulong max(ulong a, ulong b)
{
    return a >= b ? a : b;
}

ulong ipow(ulong base, ulong exp)
{
    ulong result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }
    return result;
}

ulong square(ulong x)
{
    return ipow(x, 2);
}

int is_prime(ulong n)
{
    if (n <= 3)
        return n > 1;
    else if (n % 2 == 0 || n % 3 == 0)
        return 0;
    ulong i = 5;
    while (i * i <= n)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
        i += 6;
    }
     return 1;
}


ulong pgcd(ulong a, ulong b)
{
    if (b == 0)
        return a;
    return pgcd(a, a % b);
}


// RSA

ulong prime_founder(ulong a, ulong debut)
{
    if (debut > a)
        return -1;
    if (pgcd(a, debut) == 1)
        return debut;
    return prime_founder(a, debut + 1);
}

ulong mod_founder(ulong e, ulong phi, ulong d)
{
    if (d > phi)
        return -1;
    if ((e * d) % phi == 1)
        return d;
    return mod_founder(e, phi, d + 1);
}

ulong *gen_public_key(ulong p, ulong q)
{
    ulong n = p * q;
    ulong phi = (p - 1) * (q - 1);
    ulong e = prime_founder(phi, min(p, q));

    ulong *public = malloc(sizeof(ulong) * 2);
    public[0] = e;
    public[1] = n;
    return public;
}

ulong *gen_private_key(ulong p, ulong q, ulong e)
{
    ulong n = p * q;
    ulong phi = (p - 1) * (q - 1);
    ulong d = mod_founder(e, phi, min(p, q));
    
    ulong *private = malloc(sizeof(ulong) * 2);
    private[0] = d;
    private[1] = n;
    return private;
}

ulong ulong_single_encode_rsa(ulong *public, ulong c)
{
    ulong n = public[1];
    ulong e = public[0];
    return ipow(c, e) % n;
}

ulong ulong_single_decode_rsa(ulong *private, ulong c)
{
    ulong n = private[1];
    ulong d = private[0];
    return ipow(c, d) % n;
}

void ulong_encode_rsa(ulong *public, ulong *data, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        data[i] = ulong_single_encode_rsa(public, data[i]);
}

void ulong_decode_rsa(ulong *private, ulong *data, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        data[i] = ulong_single_decode_rsa(private, data[i]);
}
