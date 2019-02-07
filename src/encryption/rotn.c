#include <stdlib.h>
#include <string.h>

#include "rotn.h"

void ROTN_encrypt(char *data, int key)
{
    size_t len = strlen(data);
    for (size_t i = 0; i < len; ++i)
    {
        data[i] += key;
    }
}

void ROTN_decrypt(char *data, int key)
{
    ROTN_encrypt(data, -key);
}
