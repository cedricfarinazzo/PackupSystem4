#include <stddef.h>
#include <unistd.h>
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

int ROTN_encrypt_file(int fin, int fout, int key)
{
    char buffer[BUFFER_SIZE + 1];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, BUFFER_SIZE)) > 0 && eout != -1)
    {
        buffer[BUFFER_SIZE] = 0;
        ROTN_encrypt(buffer, key);
        eout = write(fout, buffer, ein);
    }

    if (ein == -1 || eout == -1)
        return -1;
    return 0;
}

int ROTN_decrypt_file(int fin, int fout, int key)
{
    char buffer[BUFFER_SIZE + 1];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, BUFFER_SIZE)) > 0 && eout != -1)
    {
        buffer[BUFFER_SIZE] = 0;
        ROTN_decrypt(buffer, key);
        eout = write(fout, buffer, ein);
    }

    if (ein == -1 || eout == -1)
        return -1;
    return 0;
}
