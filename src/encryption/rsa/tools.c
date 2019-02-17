#include <stdlib.h>
#include <time.h>

#include "tools.h"

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

ulong pgcd(ulong  a, ulong b)
{
    while (b != 0)
    {
        ulong tmp = a%b;
        a = b;
        b = tmp;
    }
    return a;
}

ulong rand_a_b(ulong a, ulong b)
{
    return rand() % (b - a) + a;
}
