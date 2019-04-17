#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "aes.h"
#include "hashpass.h"

#include "aes_file.h"

int AES_encrypt_fd(int fin, int fout, char *pass)
{
    unsigned char *key = AES_keyFromPass(pass, strlen(pass)); // Generate aes key
    struct AES_ctx *ctx = AES_init(key, strlen((char*)key)); // Initialize context
    
    char buffer[AES_BUFFER_SIZE];
    int ein = 0, eout = 0;
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
    
    char buffer[AES_BUFFER_SIZE];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, AES_BUFFER_SIZE)) > 0 && eout != -1)
    {
        unsigned char *decrypt = NULL;
        size_t outlen = AES_decrypt(ctx, (unsigned char*)buffer, ein, &decrypt); // decrypt data with ctx. write outlen byte on decrypt
        eout = write(fout, decrypt, outlen);
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
