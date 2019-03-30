#include <unistd.h>
#include "aes.h"
#include "hashpass.h"

#include "aes_file.h"

int AES_encrypt_file(int fin, int fout, char *pass)
{
    unsigned char *key = AES_keyFromPass(pass, strlen(pass)); // Generate aes key
    struct AES_ctx *ctx = AES_init(key, strlen((char*)key)); // Initialize context
    
    char buffer[BUFFER_SIZE];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, BUFFER_SIZE)) > 0 && eout != -1)
    {
        unsigned char *encrypt = NULL;
        size_t outlen = AES_encrypt(ctx, (unsigned char*)buffer, BUFFER_SIZE, &encrypt); // encrypt data with ctx. write outlen byte on encrypt
        eout = write(fout, encrypt, outlen);
        free(encrypt);
    }

    AES_ctx_free(ctx); //free aes ctx
    free(key);
    
    if (ein == -1 || eout == -1)
        return -1;
    return 0;
}


int AES_decrypt_file(int fin, int fout, char *pass)
{
    unsigned char *key = AES_keyFromPass(pass, strlen(pass)); // Generate aes key
    struct AES_ctx *ctx = AES_init(key, strlen((char*)key)); // Initialize context
    
    char buffer[BUFFER_SIZE];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, BUFFER_SIZE)) > 0 && eout != -1)
    {
        unsigned char *decrypt = NULL;
        size_t outlen = AES_decrypt(ctx, (unsigned char*)buffer, BUFFER_SIZE, &decrypt); // decrypt data with ctx. write outlen byte on decrypt
        eout = write(fout, decrypt, outlen);
        free(decrypt);
    }

    AES_ctx_free(ctx); //free aes ctx
    free(key);

    if (ein == -1 || eout == -1)
        return -1;
    return 0;
}
