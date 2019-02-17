#include <stddef.h>
#include "rotn.h"

void ROTN_encrypt(char *data, int key)
{
    for (size_t i = 0; data[i] != '\0'; ++i)
        data[i] += key;
}

void ROTN_decrypt(char *data, int key)
{
    ROTN_encrypt(data, -key);
}
