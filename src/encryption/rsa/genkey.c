#include <stdlib.h>
#include "tools.h"
#include "genkey.h"

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

ulong *RSA_gen_public_key(ulong p, ulong q)
{
    ulong n = p * q;
    ulong phi = (p - 1) * (q - 1);
    ulong e = prime_founder(phi, min(p, q));

    ulong *public = malloc(sizeof(ulong) * 2);
    public[0] = e;
    public[1] = n;
    return public;
}

ulong *RSA_gen_private_key(ulong p, ulong q, ulong *public)
{
    ulong e = public[0];
    ulong n = p * q;
    ulong phi = (p - 1) * (q - 1);
    ulong d = mod_founder(e, phi, min(p, q));
    
    ulong *private = malloc(sizeof(ulong) * 2);
    private[0] = d;
    private[1] = n;
    return private;
}
