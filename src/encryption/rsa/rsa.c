#include <stdlib.h>
#include <gmp.h>
#include "genkey.h"
#include "tools.h"


void single_encode_rsa(struct RSA_publickey *public, mpz_t c, mpz_t r)
{
    mpz_powm_sec(r, c, *(public->e), *(public->n));
    //ipow(c, *(public->e), r);
    //mpz_mod(r, r, *(public->n));
}


void single_decode_rsa(struct RSA_privatekey *private, mpz_t c, mpz_t r)
{
    mpz_powm_sec(r, c, *(private->d), *(private->n));
    //ipow(c, *(private->d), r);
    //mpz_mod(r, r, *(private->n));
}


mpz_t *RSA_encode(struct RSA_publickey *public, unsigned char *data, size_t len)
{
    mpz_t *encode = malloc(sizeof(mpz_t) * len);
    for (size_t i = 0; i < len; ++i)
    {   
        mpz_t c;
        mpz_init_set_ui(c, (unsigned char) data[i]);
        mpz_init(encode[i]);
        single_encode_rsa(public, c, encode[i]);
        mpz_clear(c);
    }
    return encode;
}

unsigned char *RSA_decode(struct RSA_privatekey *private, mpz_t *data, size_t len)
{
    unsigned char *decode = calloc(len + 1, sizeof(unsigned char));
    for (size_t i = 0; i < len; ++i)
    {   
        mpz_t c;
        mpz_init(c);
        single_decode_rsa(private, data[i], c);
        unsigned char cc = (unsigned char) mpz_get_ui(c);
        mpz_clear(c);
        decode[i] = cc;
    }
    return decode;
}
