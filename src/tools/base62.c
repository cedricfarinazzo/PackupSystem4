#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "base62.h"

void byte_to_binary(int x, char b[9])
{
    b[0] = '\0';
    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
}

char *base62_encode(char *in, size_t len, size_t *olen)
{
    *olen = 0;
    char *out = malloc(sizeof(char) * *olen);
    uint8_t ind = 0;
    
    char ascii;
    char bits[9];
    size_t countb = 0;
    
    char a = 0; size_t nba = 0;
    size_t i = 0;
    
    while (i < len)
    {
        if (countb == 9)
        {
            ascii = in[i];
            byte_to_binary(ascii, bits);
            printf("%s\n", bits);
            countb = 0;
            ++i;
        }

        if (nba == 6)
        {
            printf("%d\n", a);
            a = 0; nba = 0;
        }
        
        a <<= 1; a |= ('1' == bits[countb] ? 1 : 0);
        ++countb; ++nba;
    }
    /*
    for (i = 0; i < len; ++i)
    {
        
        ascii = in[i];
        byte_to_binary(ascii, bits);
        countb = 0;
        printf("%s\n", bits);
        a = 0; nba = 0;
        for (countb = 0; countb < 6; ++countb, ++nba)
        {
            a <<= 1; a |= ('1' == bits[countb] ? 1 : 0);
        }
        printf("%d\n", a);
    }
*/
    
    
    ++(*olen);
    out = realloc(out, sizeof(char) * *olen);
    out[*olen - 1] = 0;
    return out;
}

char *base62_decode(char *in, size_t len, size_t *olen)
{
    *olen = 0;
    char *out = malloc(sizeof(char) * *olen);
    
    
    ++(*olen);
    out = realloc(out, sizeof(char) * *olen);
    out[*olen - 1] = 0;
    return out;
}
