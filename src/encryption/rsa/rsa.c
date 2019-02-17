#include <stdlib.h>
#include "genkey.h"
#include "tools.h"

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

ulong *RSA_encode(ulong *public, char *data, size_t len)
{
    ulong *encode = malloc(sizeof(ulong) * len);
    for (size_t i = 0; i < len; ++i)
        encode[i] = ulong_single_encode_rsa(public, (ulong)data[i]);
    return encode;
}

char *RSA_decode(ulong *private, ulong *data, size_t len)
{
    char *decode = malloc(sizeof(char) * len);
    for (size_t i = 0; i < len; ++i)
        decode[i] = (char)ulong_single_decode_rsa(private, data[i]);
    return decode;
}

