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
    int ein = 42, eout = 0;

    while (ein > 0 && eout != -1)
    {
        char buff[128];
        for (size_t i = 0; i < 128; ++i) buff[i] = 0;
        ein = read(fin, buff, 128);
    
        mpz_t t, r; mpz_init(t); mpz_init(r);
        mpz_import(t, 128, 1, 1, 0, 0, buff);

        single_encode_rsa(pubk, t, r);

        char *outputchunk =  mpz_get_str(NULL, 16, r);
        size_t l = mpz_sizeinbase (r, 16) + 2;
        
        eout = write(fout, outputchunk, l-1);
        free(outputchunk);

        mpz_clear(r); mpz_clear(t);
    }

    if (ein == -1)
        return RSA_ERROR_CANNOT_READ_FD;
    if (eout == -1)
        return RSA_ERROR_CANNOT_WRITE_FD;
    return RSA_OK;
}


int RSA_decode_fd(int fin, int fout, struct RSA_privKey *privk)
{
    int ein = 42, eout = 0;
    while(ein > 0 && eout != -1)
    {
        char buff[258];
        for (size_t i = 0; i < 258; ++i) buff[i] = 0;
        ein = read(fin, buff, 258);

        mpz_t t, r; mpz_init(t); mpz_init(r);
        mpz_set_str(t, buff, 16);

        single_decode_rsa(privk, t, r);

        size_t l;
        char *outputchunk = mpz_export(NULL, &l, 1, 1, 0, 0, r);
        
        eout = write(fout, outputchunk, l);
        
        free(outputchunk);
        mpz_clear(r); mpz_clear(t);
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

