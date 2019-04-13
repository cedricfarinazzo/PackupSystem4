#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "base2.h"
#include "genkey.h"
#include "tools.h"

#define RSA_BUFFER_LEN 4096

void single_encode_rsa(struct RSA_pubKey *public, mpz_t c, mpz_t r)
{
    //ipow(c, *(public->e), r);
    //mpz_mod(r, r, *(public->n));
    //mpz_powm(r, c, *(public->e), *(public->n));
    mpz_powm_sec(r, c, *(public->e), *(public->n));
}


void single_decode_rsa(struct RSA_privKey *private, mpz_t c, mpz_t r)
{
    //ipow(c, *(private->d), r);
    //mpz_mod(r, r, *(private->n));
    //mpz_powm(r, c, *(private->d), *(private->n));
    mpz_powm_sec(r, c, *(private->d), *(private->n));
}


unsigned char *RSA_encode(struct RSA_pubKey *public, unsigned char *data, size_t len, size_t *rlen)
{
    char *output = malloc(0 * sizeof(char*));
    *rlen = 0;

    for (size_t i = 0; i < len; i+= 128)
    {
        char buff[128];
        strncpy(buff, (char*)(data + i), 128);
    
        mpz_t t, r; mpz_init(t); mpz_init(r);
        mpz_import(t, 128, 1, 1, 0, 0, buff);

        single_encode_rsa(public, t, r);

        char *outputchunk =  mpz_get_str(NULL, 16, r);
        size_t l = mpz_sizeinbase (r, 16) + 2;
        
        *rlen += l;
        output = realloc(output, *rlen * sizeof(char));
        strncpy((output + *rlen - l), outputchunk, l);
        
        free(outputchunk);

        mpz_clear(r); mpz_clear(t);
    }

    return (unsigned char*)output;
}


unsigned char *RSA_decode(struct RSA_privKey *private, unsigned char *data, size_t len, size_t *rlen)
{
    char *output = malloc(0 * sizeof(char*));
    *rlen = 0;
    
    for (size_t i = 0; i < len; i+= 258)
    {
        char buff[258];
        strncpy(buff, (char*)(data + i), 258);

        mpz_t t, r; mpz_init(t); mpz_init(r);
        mpz_set_str(t, buff, 16);
        //mpz_import(t, len, 1, 1, 0, 0, data);

        single_decode_rsa(private, t, r);

        size_t l;
        char *outputchunk = mpz_export(NULL, &l, 1, 1, 0, 0, r);
        
        *rlen += l;
        output = realloc(output, *rlen * sizeof(char));
        strncpy((output + *rlen - l), outputchunk, l);
        
        free(outputchunk);
        mpz_clear(r); mpz_clear(t);
    }
    (*rlen)++;
    output = realloc(output, *rlen * sizeof(char));
    output[*rlen - 1] = 0;
    return (unsigned char*)output;
}
