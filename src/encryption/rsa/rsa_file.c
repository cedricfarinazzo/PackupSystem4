#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "genkey.h"
#include "rsa.h"

#include "rsa_file.h"


int RSA_encode_fd(int fin, int fout, struct RSA_pubKey *pubk)
{
    struct stat s;
    fstat (fin, &s);
    unsigned long size = s.st_size;
    
    char buffs[32];
    for (size_t i = 0; i < 32; ++i) buffs[i] = 0;
    sprintf(buffs, "%ld", size);
    int eout = write(fout, buffs, 32);

    char buffer[RSA_BUFFER_SIZE];
    int ein = 0;
    while ((ein = read(fin, buffer, RSA_BUFFER_SIZE)) > 0 && eout != -1)
    {
        size_t outlen;
        unsigned char *encrypt = RSA_encode(pubk, (unsigned char*)buffer, ein, &outlen); 
        eout = write(fout, encrypt, outlen);
        free(encrypt);
    }
    
    if (ein == -1)
        return RSA_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return RSA_ERROR_CANNOT_WRITE_FD;
    return RSA_OK;
}


int RSA_decode_fd(int fin, int fout, struct RSA_privKey *privk)
{
    char buffs[32 + 1]; buffs[32] = 0;
    read(fin, buffs, 32);
    unsigned long size = atol(buffs);
    
    char buffer[RSA_BUFFER_SIZE];
    int ein = 0, eout = 0;
    while ((ein = read(fin, buffer, RSA_BUFFER_SIZE)) > 0 && eout != -1)
    {
        size_t outlen;
        unsigned char *decrypt = RSA_decode(privk, (unsigned char*)buffer, ein, &outlen); 
        if (size < RSA_BUFFER_SIZE)
            outlen = size;
        eout = write(fout, decrypt, outlen);
        size -= outlen;
        free(decrypt);
    }

    if (ein == -1)
        return RSA_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return RSA_ERROR_CANNOT_WRITE_FD;
    return RSA_OK;
}


int RSA_encode_file(char *in, char *out, struct RSA_pubKey *pubk)
{
    if (in == NULL || out == NULL)
        return RSA_ERROR_NULL_PATH;
    if (strcmp(in, "") == 0 || strcmp(out, "") == 0)
        return RSA_ERROR_EMPTY_PATH;
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return RSA_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_WRONLY | O_CREAT, 0644);
    if (fout < 0) {
        close(fin);
        return RSA_ERROR_CANNOT_OPEN_FD;
    }
    int r = RSA_encode_fd(fin, fout, pubk);
    close(fin);
    close(fout);
    return r;
}

int RSA_decode_file(char *in, char *out, struct RSA_privKey *privk)
{
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return RSA_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_WRONLY | O_CREAT, 0644);
    if (fout < 0) {
        close(fin);
        return RSA_ERROR_CANNOT_OPEN_FD;
    }
    int r = RSA_decode_fd(fin, fout, privk);
    close(fin);
    close(fout);
    return r;
}

