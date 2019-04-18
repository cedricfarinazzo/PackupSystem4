#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <sys/mman.h>
#include <string.h>
#include "genkey.h"
#include "rsa.h"
#include "rsa_file.h"

int RSA_encode_fd(int fin, int fout, struct RSA_pubKey *pubk)
{
    struct stat fileInfo;
    fstat(fin, &fileInfo);

    FILE *f = fdopen(fout, "w+");
    char *data = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fin, 0);
    size_t len = fileInfo.st_size;    

    size_t buffsize = 8;

    for (size_t i = 0; i < len; i+= buffsize)
    {
        char buff[buffsize];
        strncpy(buff, (char*)(data + i), buffsize);
    
        mpz_t t, r; mpz_init(t); mpz_init(r);
        mpz_import(t, buffsize, 1, 1, 0, 0, buff);

        single_encode_rsa(pubk, t, r);
        if (mpz_out_raw(f, r) == 0)
           return RSA_ERROR_CANNOT_WRITE_FD;

        mpz_clear(r); mpz_clear(t);
    }
    fclose(f);

    return RSA_OK;
}


int RSA_decode_fd(int fin, int fout, struct RSA_privKey *privk)
{
    int ein;
    mpz_t r, t; mpz_init(r); mpz_init(t);
    FILE *f = fdopen(fin, "r");
    while ((ein = mpz_inp_raw(t, f)) > 0)
    {
        single_decode_rsa(privk, t, r);
        size_t l;
        char *outputchunk = mpz_export(NULL, &l, 1, 1, 0, 0, r);
        write(fout, outputchunk, l);
        free(outputchunk);
    }
    fclose(f);
    mpz_clear(r); mpz_clear(t);
    return RSA_OK;
}


int RSA_encode_file(char *in, char *out, struct RSA_pubKey *pubk)
{
    if (in == NULL || out == NULL)
        return RSA_ERROR_NULL_PATH;
    int fin = open(in, O_RDONLY);
    if (fin < 0)
        return RSA_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_RDWR | O_TRUNC | O_CREAT, 0666);
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
    int fin = open(in, O_RDONLY, 0666);
    if (fin < 0)
        return RSA_ERROR_CANNOT_OPEN_FD;
    int fout = open(out, O_RDWR | O_TRUNC | O_CREAT, 0666);
    if (fout < 0) {
        close(fin);
        return RSA_ERROR_CANNOT_OPEN_FD;
    }
    int r = RSA_decode_fd(fin, fout, privk);
    close(fin);
    close(fout);
    return r;
}

