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


mpz_t *RSA_encode(struct RSA_pubKey *public, unsigned char *data, size_t len, size_t *rlen)
{
    size_t elen;
    char *edata = base2_encode((char*)data, len, &elen); 
    *rlen = 0;
    mpz_t *result = malloc(sizeof(mpz_t) * *rlen);
    char *p = edata;
    for (size_t i = 0; p < edata + elen; p += RSA_BUFFER_LEN, ++i)
    {
        ++(*rlen);
        result = realloc(result, sizeof(mpz_t) * *rlen);
        
        size_t blen = RSA_BUFFER_LEN;
        char buff[blen];
        strncpy(buff, (char*)p, blen);
    
        mpz_init(result[i]);
        mpz_set_str(result[i], buff, 2);
        
        single_encode_rsa(public, result[i], result[i]);
    }
    free(edata);
    return result;
}


unsigned char *RSA_decode(struct RSA_privKey *private, mpz_t *data, size_t len, size_t *rlen)
{
    *rlen = 0;
    char *result = malloc(sizeof(char) * *rlen);
    for (size_t i = 0; i < len; ++i)
    {
        
        mpz_t dec; mpz_init(dec);
        single_decode_rsa(private, data[i], dec);
           
        char *buff = mpz_get_str(NULL, 2, dec);
        size_t t = strlen(buff);

        (*rlen) += t;
        result = realloc(result, sizeof(char) * *rlen);
        
        strncpy((char*)(result + *rlen - t), buff, t);
        
        free(buff);
        mpz_clear(dec);
    }

    size_t align = (-*rlen % 8) + 8; align %= 8;
    size_t lou = *rlen + align + 1;
    char *out = malloc(sizeof(char) * lou);
    size_t ea = 0;
    for (; ea < align; ++ea)
        out[ea] = '0';
    strncpy(out + align, result, *rlen);
    out[lou - 1] = 0;
    free(result);

    size_t dlen;
    char *dr = base2_decode((char*)out, lou - 1, &dlen);
    
    free(out);
    *rlen = dlen;
    
    return (unsigned char*)dr;
}
