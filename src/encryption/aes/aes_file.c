#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "aes.h"
#include "hashpass.h"

#include "aes_file.h"

int AES_encrypt_fd(int fin, int fout, char *pass)
{
    unsigned char *key = AES_keyFromPass(pass, strlen(pass)); // Generate aes key
    struct AES_ctx *ctx = AES_init(key, strlen((char*)key)); // Initialize context
    
    struct stat s;
    fstat (fin, &s);
    unsigned long size = s.st_size;
    
    char buffs[32];
    for (size_t i = 0; i < 32; ++i) buffs[i] = 0;
    sprintf(buffs, "%ld", size);
    int eout = write(fout, buffs, 32);

    char buffer[AES_BUFFER_SIZE];
    int ein = 0;
    while ((ein = read(fin, buffer, AES_BUFFER_SIZE)) > 0 && eout != -1)
    {
        unsigned char *encrypt = NULL;
        size_t outlen = AES_encrypt(ctx, (unsigned char*)buffer, ein, &encrypt); // encrypt data with ctx. write outlen byte on encrypt
        eout = write(fout, encrypt, outlen);
        free(encrypt);
    }

    AES_ctx_free(ctx); //free aes ctx
    free(key);
    
    if (ein == -1)
        return AES_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return AES_ERROR_CANNOT_WRITE_FD;
    return AES_OK;
}


int AES_decrypt_fd(int fin, int fout, char *pass)
{
    unsigned char *key = AES_keyFromPass(pass, strlen(pass)); // Generate aes key
    struct AES_ctx *ctx = AES_init(key, strlen((char*)key)); // Initialize context
    
    char buffs[32 + 1]; buffs[32] = 0;
    read(fin, buffs, 32);
    unsigned long size = atol(buffs);
    
    char buffer[AES_BUFFER_SIZE];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, AES_BUFFER_SIZE)) > 0 && eout != -1)
    {
        unsigned char *decrypt = NULL;
        size_t outlen = AES_decrypt(ctx, (unsigned char*)buffer, ein, &decrypt); // decrypt data with ctx. write outlen byte on decrypt
        if (size < AES_BUFFER_SIZE)
            outlen = size;
        eout = write(fout, decrypt, outlen);
        size -= outlen;
        free(decrypt);
    }

    AES_ctx_free(ctx); //free aes ctx
    free(key);

    if (ein == -1)
        return AES_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return AES_ERROR_CANNOT_WRITE_FD;
    return AES_OK;
}


int AES_encrypt_file(char *in, char *out, char *pass)
{
    if (in == NULL || out == NULL)
        return AES_ERROR_NULL_PATH;
    if (strcmp(in, "") == 0 || strcmp(out, "") == 0)
        return AES_ERROR_EMPTY_PATH;
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return AES_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_WRONLY | O_CREAT, 0644);
    if (fout < 0) {
        close(fin);
        return AES_ERROR_CANNOT_OPEN_FD;
    }
    int r = AES_encrypt_fd(fin, fout, pass);
    close(fin);
    close(fout);
    return r;
}

int AES_decrypt_file(char *in, char *out, char *pass)
{
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return AES_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_WRONLY | O_CREAT, 0644);
    if (fout < 0) {
        close(fin);
        return AES_ERROR_CANNOT_OPEN_FD;
    }
    int r = AES_decrypt_fd(fin, fout, pass);
    close(fin);
    close(fout);
    return r;
}
