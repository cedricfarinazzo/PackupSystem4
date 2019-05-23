#include <stdio.h>
#include <string.h>
#include <fcntl.h>
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

int VIGENERE_encrypt_fd(int fin, int fout, char *key)
{
    char buffer[VIGENERE_BUFFER_SIZE + 1];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, VIGENERE_BUFFER_SIZE)) > 0 && eout != -1)
    {
        buffer[VIGENERE_BUFFER_SIZE] = 0;
        VIGENERE_encrypt(buffer, key);
        eout = write(fout, buffer, ein);
    }

    if (ein == -1)
        return VIGENERE_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return VIGENERE_ERROR_CANNOT_WRITE_FD;
    return VIGENERE_OK;
}

int VIGENERE_decrypt_fd(int fin, int fout, char *key)
{
    char buffer[VIGENERE_BUFFER_SIZE + 1];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, VIGENERE_BUFFER_SIZE)) > 0 && eout != -1)
    {
        buffer[VIGENERE_BUFFER_SIZE] = 0;
        VIGENERE_decrypt(buffer, key);
        eout = write(fout, buffer, ein);
    }

    if (ein == -1)
        return VIGENERE_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return VIGENERE_ERROR_CANNOT_WRITE_FD;
    return VIGENERE_OK;
}

int VIGENERE_encrypt_stream(FILE *fin, FILE *fout, char *key)
{
    int in = fileno(fin);
    int out = fileno(fout);
    if (in == -1 || out == -1)
        return VIGENERE_ERROR_CANNOT_OPEN_FD;
    int e = VIGENERE_encrypt_fd(in, out, key);
    close(in);
    close(out);
    return e;
}

int VIGENERE_decrypt_stream(FILE *fin, FILE *fout, char *key)
{
    int in = fileno(fin);
    int out = fileno(fout);
    if (in == -1 || out == -1)
        return VIGENERE_ERROR_CANNOT_OPEN_FD;
    int e = VIGENERE_decrypt_fd(in, out, key);
    close(in);
    close(out);
    return e;
}
int VIGENERE_encrypt_file(char *in, char *out, char *key)
{
    if (in == NULL || out == NULL)
        return VIGENERE_ERROR_NULL_PATH;
    if (strcmp(in, "") == 0 || strcmp(out, "") == 0)
        return VIGENERE_ERROR_EMPTY_PATH;
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return VIGENERE_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_WRONLY | O_CREAT, 0644);
    if (fout < 0) {
        close(fin);
        return VIGENERE_ERROR_CANNOT_OPEN_FD;
    }
    int r = VIGENERE_encrypt_fd(fin, fout, key);
    close(fin);
    close(fout);
    return r;
}

int VIGENERE_decrypt_file(char *in, char *out, char *key)
{
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return VIGENERE_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_WRONLY | O_CREAT, 0644);
    if (fout < 0) {
        close(fin);
        return VIGENERE_ERROR_CANNOT_OPEN_FD;
    }
    int r = VIGENERE_decrypt_fd(fin, fout, key);
    close(fin);
    close(fout);
    return r;
}
