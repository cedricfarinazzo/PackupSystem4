#include <stdlib.h>
#include <string.h>

#include "vigenere.h"

void VIGENERE_encrypt(char *data, char *key)
{
    size_t k = 0;
    for (size_t i = 0; data[i] != '\0'; ++i)
    {
        char c = key[k];
        data[i] += c; 
        ++k;
        if (key[k] == '\0')
            k = 0;
    }
}

void VIGENERE_decrypt(char *data, char *key)
{

    size_t k = 0;
    for (size_t i = 0; data[i] != '\0'; ++i)
    {
        data[i] -= key[k]; 
        ++k;
        if (key[k] == '\0')
            k = 0;
    }
}
