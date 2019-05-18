#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
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

int ROTN_encrypt_fd(int fin, int fout, int key)
{
    char buffer[ROTN_BUFFER_SIZE + 1];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, ROTN_BUFFER_SIZE)) > 0 && eout != -1)
    {
        buffer[ROTN_BUFFER_SIZE] = 0;
        ROTN_encrypt(buffer, key);
        eout = write(fout, buffer, ein);
    }

    if (ein == -1)
        return ROTN_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return ROTN_ERROR_CANNOT_WRITE_FD;
    return ROTN_OK;
}

int ROTN_decrypt_fd(int fin, int fout, int key)
{
    char buffer[ROTN_BUFFER_SIZE + 1];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, ROTN_BUFFER_SIZE)) > 0 && eout != -1)
    {
        buffer[ROTN_BUFFER_SIZE] = 0;
        ROTN_decrypt(buffer, key);
        eout = write(fout, buffer, ein);
    }

    if (ein == -1)
        return ROTN_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return ROTN_ERROR_CANNOT_WRITE_FD;
    return ROTN_OK;
}

int ROTN_encrypt_stream(FILE *fin, FILE *fout, int key)
{
    int in = fileno(fin);
    int out = fileno(fout);
    if (in == -1 || out == -1)
        return ROTN_ERROR_CANNOT_OPEN_FD;
    int e = ROTN_encrypt_fd(in, out, key);
    close(in);
    close(out);
    return e;
}

int ROTN_decrypt_stream(FILE *fin, FILE *fout, int key)
{
    int in = fileno(fin);
    int out = fileno(fout);
    if (in == -1 || out == -1)
        return ROTN_ERROR_CANNOT_OPEN_FD;
    int e = ROTN_decrypt_fd(in, out, key);
    close(in);
    close(out);
    return e;
}

int ROTN_encrypt_file(char *in, char *out, int key)
{
    if (in == NULL || out == NULL)
        return ROTN_ERROR_NULL_PATH;
    if (strcmp(in, "") == 0 || strcmp(out, "") == 0)
        return ROTN_ERROR_EMPTY_PATH;
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return ROTN_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_WRONLY | O_CREAT, 0644);
    if (fout < 0) {
        close(fin);
        return ROTN_ERROR_CANNOT_OPEN_FD;
    }
    int r = ROTN_encrypt_fd(fin, fout, key);
    close(fin);
    close(fout);
    return r;
}

int ROTN_decrypt_file(char *in, char *out, int key)
{
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return ROTN_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_WRONLY | O_CREAT, 0644);
    if (fout < 0) {
        close(fin);
        return ROTN_ERROR_CANNOT_OPEN_FD;
    }
    int r = ROTN_decrypt_fd(fin, fout, key);
    close(fin);
    close(fout);
    return r;
}
