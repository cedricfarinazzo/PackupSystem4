#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include "genkey.h"
#include "rsa.h"

#include "rsa_file.h"


int RSA_encode_fd(int fin, int fout, struct RSA_pubKey *pubk)
{
    struct stat fileInfo;
    fstat(fin, &fileInfo);

    char *data = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fin, 0);
    
    size_t len;
    unsigned char *en = RSA_encode(pubk, (unsigned char*)data, fileInfo.st_size, &len);
    munmap(data, fileInfo.st_size);
    len = strlen((char*)en);
    unsigned char *tmp = en;
    while (tmp < en + len)
    {
        size_t w = en - tmp + len > 512 ? 512 : en - tmp + len;
        if (write(fout, tmp, w) == -1)
        {
            free(en);
            return RSA_ERROR_CANNOT_WRITE_FD;
        }
        tmp += 512;
    }
    free(en);
    return RSA_OK;
}


int RSA_decode_fd(int fin, int fout, struct RSA_privKey *privk)
{

    struct stat fileInfo;
    fstat(fin, &fileInfo);

    char *data = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fin, 0);
    
    size_t len;
    unsigned char *de = RSA_decode(privk, (unsigned char*)data, fileInfo.st_size, &len);
    munmap(data, fileInfo.st_size);
    
    unsigned char *tmp = de;
    len = strlen((char*)de);
    while (tmp < de + len)
    {
        size_t w = de - tmp + len > 512 ? 512 : de - tmp + len;
        if (write(fout, tmp, w) == -1)
        {
            free(de);
            return RSA_ERROR_CANNOT_WRITE_FD;
        }
        tmp += 512;
    }
    free(de);
    return RSA_OK;
}


int RSA_encode_file(char *in, char *out, struct RSA_pubKey *pubk)
{
    if (in == NULL || out == NULL)
        return RSA_ERROR_NULL_PATH;
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

