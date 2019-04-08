#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int VIGENERE_encrypt_file(int fin, int fout, char *key)
{
    char buffer[BUFFER_SIZE + 1];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, BUFFER_SIZE)) > 0 && eout != -1)
    {
        buffer[BUFFER_SIZE] = 0;
        VIGENERE_encrypt(buffer, key);
        eout = write(fout, buffer, ein);
    }

    if (ein == -1 || eout == -1)
        return -1;
    return 0;
}

int VIGENERE_decrypt_file(int fin, int fout, char *key)
{
    char buffer[BUFFER_SIZE + 1];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, BUFFER_SIZE)) > 0 && eout != -1)
    {
        buffer[BUFFER_SIZE] = 0;
        VIGENERE_decrypt(buffer, key);
        eout = write(fout, buffer, ein);
    }

    if (ein == -1 || eout == -1)
        return -1;
    return 0;
}
