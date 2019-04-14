#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "base2.h"

unsigned long base2_bin2dec(const char *binary)
{
    unsigned long decimal = 0;
    do {
        if ( *binary == '0' ) {
            decimal <<= 1;
        } else if ( *binary == '1' ) {
            decimal <<= 1;
            decimal += 1;
        } else if ( *binary == ' ' ) {
            ;
        } else {
            break;
        }
    } while ( *(++binary) != 0 );
    return decimal;
}

void base2_byte_to_binary(int x, char b[9])
{
    for (size_t i = 0; i < 9; ++i)
        b[i] = '0';
    b[0] = '\0';
    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
}

char *base2_encode(char *in, size_t len, size_t *olen)
{
    *olen = len * 8;
    char *out = malloc(sizeof(char) * *olen);
    
    size_t i = 0, k = 0;
    while (i < len)
    {
        char b[9];
        base2_byte_to_binary(in[i], b);
        for (size_t j = 0; j < 8; ++j, ++k)
            out[k] = b[j];
        ++i;
    }
    
    ++(*olen);
    out = realloc(out, sizeof(char) * *olen);
    out[*olen - 1] = 0;
    return out;
}

char *base2_decode(char *in, size_t len, size_t *olen)
{
    *olen = len / 8;
    char *out = malloc(sizeof(char) * *olen);

    size_t i = 0, j = 0;;
    while (i < len)
    {
        char b[9];
        for (size_t j = 0; j < 8 && i < len; ++j, ++i)
        {
            b[j] = in[i];
        }
        b[8] = 0;

        out[j] = base2_bin2dec(b);
        ++j;
    }

    ++(*olen);
    out = realloc(out, sizeof(char) * *olen);
    out[*olen - 1] = 0;
    return out;
}
