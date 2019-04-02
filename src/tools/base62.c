#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "base62.h"

char base62_table[62] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
char nth62(char a)
{
    for (size_t i = 0; i < 62; ++i)
        if (a == base62_table[i])
            return i;
    return -1;
}
unsigned long bin2dec(const char *binary)
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

void byte_to_binary(int x, char b[9])
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
    
    ascii = in[i];
    byte_to_binary(ascii, bits);
    //printf("%s\n", bits);
    countb = 0;
            ++i;
    while (1)
    {
        if (countb == 8)
        {
            if (i >= len)
                break;
            ascii = in[i];
            byte_to_binary(ascii, bits);
            countb = 0;
            ++i;
        }

        if (nba == 6)
        {
            //printf("%d\n", a);
            ++(*olen);
            out = realloc(out, sizeof(char) * *olen);
            out[*olen - 1] = base62_table[a];
            a = 0; nba = 0;
        }
        a <<= 1; a |= ('1' == bits[countb] ? 1 : 0);
        ++countb; ++nba;
    }
    printf("\n\n%d    %d\n\n", countb, nba);

    if (nba != 0)
    {
        for (nba; nba < 6; ++nba)
            a <<= 1;
        ++(*olen);
        out = realloc(out, sizeof(char) * *olen);
        out[*olen - 1] = base62_table[a];
        a = 0; nba = 0;
    }
    (*olen) += 1;
    out = realloc(out, sizeof(char) * *olen);
    out[*olen - 1] = 0;
    return out;
}

char *base62_decode(char *in, size_t len, size_t *olen)
{
    *olen = 0;
    char *out = malloc(sizeof(char) * *olen);
    
    char bits[9]; int boff = 2;
    byte_to_binary(nth62(in[0]), bits);
    char ascii[9]; int asoff = 0;
    
    size_t i = 1;
    while (1)
    {
        if (asoff == 8)
        {
            char d = bin2dec(ascii);
            printf("\nadd: %d   %c  %s", d, d, ascii);
            ++(*olen);
            out = realloc(out, sizeof(char) * *olen);
            out[*olen - 1] = d;
            asoff = 0;
        }

        if (boff == 8)
        {
            if (i >= len - 1)
                break;
            byte_to_binary(nth62(in[i]), bits);
            boff = 2;
            printf("\nread: %c %d     %s", in[i], nth62(in[i]), bits);
            ++i;
        }
        
        ascii[asoff] = bits[boff];
        ++boff; ++asoff;
    }
    
    printf("\n   %d     %d\n", asoff, boff);
    
    ++(*olen);
    out = realloc(out, sizeof(char) * *olen);
    out[*olen - 1] = 0;
    return out;
}
