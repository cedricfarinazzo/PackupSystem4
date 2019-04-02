#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "base62.h"
#include "genkey.h"
#include "tools.h"

#define RSA_BUFFER_LEN 171

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
    char *edata = base62_encode((char*)data, len, &elen);
    *rlen = 0;
    mpz_t *result = malloc(sizeof(mpz_t) * *rlen);
    char *p = edata;
    for (size_t i = 0; p < edata + elen; p += RSA_BUFFER_LEN, ++i)
    {
        ++(*rlen);
        result = realloc(result, sizeof(mpz_t) * *rlen);
        
        size_t blen = RSA_BUFFER_LEN;
        char buff[blen + 1];
        strncpy(buff, (char*)p, blen);
        buff[blen] = 0;

        mpz_init(result[i]);
        mpz_set_str(result[i], buff, 62);
        
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
        (*rlen) += RSA_BUFFER_LEN;
        result = realloc(result, sizeof(char) * *rlen);
        
        mpz_t dec; mpz_init(dec);
        single_decode_rsa(private, data[i], dec);
           
        char buff[RSA_BUFFER_LEN + 1];
        buff[RSA_BUFFER_LEN] = 0;
        mpz_get_str(buff, 62, dec);
        
        strncpy((char*)(result + *rlen - RSA_BUFFER_LEN), buff, RSA_BUFFER_LEN);
        
        mpz_clear(dec);
    }
    ++(*rlen);
    result = realloc(result, sizeof(char) * *rlen);
    result[*rlen - 1] = 0;
    
    size_t dlen;
    char *dr = base62_decode((char*)result, *rlen, &dlen);
    free(result);
    *rlen = dlen;
    
    return (unsigned char*)dr;
}
