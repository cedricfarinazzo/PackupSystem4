#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "tools.h"
#include "genkey.h"

mpz_t *findModInverse(mpz_t a, mpz_t m)
{
    mpz_t *r = malloc(sizeof(mpz_t));
    mpz_init(*r);
    mpz_invert(*r, a, m);
    return r;
}


mpz_t *generateLargeNumber(unsigned long keysize, gmp_randstate_t r_state)
{
    mpz_t *r = malloc(sizeof(mpz_t)); mpz_init(*r);
    
    unsigned long sizek = keysize/2;
    
    mpz_urandomb(*r,r_state, sizek/2);
    
    mpz_t n; mpz_init(n); mpz_ui_pow_ui(n, 2, keysize - 2);
    mpz_add(*r, *r, n);
    mpz_clear(n);
    return r;
}


mpz_t *generateLargePrime(unsigned long keysize, gmp_randstate_t r_state)
{
    mpz_t *r = malloc(sizeof(mpz_t));
    mpz_init(*r);

    unsigned long sizek = keysize/2;

    mpz_t rand_Num; mpz_init(rand_Num);
    mpz_urandomb(rand_Num ,r_state, sizek/2);
    
    mpz_t n; mpz_init(n);
    mpz_ui_pow_ui(n, 2, sizek);
    mpz_add(n, n, rand_Num);
    
    mpz_nextprime(*r, n);

    mpz_clear(rand_Num); mpz_clear(n);

    return r;
}


void RSA_generateKey(unsigned long keysize, struct RSA_privKey **privk, struct RSA_pubKey **pubk)
{
    if (keysize < 4)
        return;
    gmp_randstate_t r_state, base; 
    gmp_randinit_default(base); mpz_t tmp, ra; mpz_init(tmp); mpz_init(ra); mpz_clear(ra);
    mpz_urandomb(tmp, base, keysize);
    gmp_randinit_lc_2exp(r_state, tmp, rand(),128);
    gmp_randclear(base); mpz_clear(tmp);

    mpz_t *p = generateLargePrime(keysize, r_state);
    mpz_t *q = NULL;
    int okq = 0;
    while (!okq)
    {
        q = generateLargePrime(keysize, r_state);
        if (mpz_cmp(*q, *p) != 0)
            okq = 1;
        if (!okq) {
            mpz_clear(*q); free(q);
        }
    }

    mpz_t n; mpz_init(n); mpz_mul(n, *p, *q);

    mpz_t phi; mpz_init(phi);
    mpz_t p1; mpz_init_set(p1, *p); mpz_sub_ui(p1, p1, 1);
    mpz_t q1; mpz_init_set(q1, *q);  mpz_sub_ui(q1, q1, 1);
    mpz_mul(phi, p1, q1); mpz_clear(p1); mpz_clear(q1);
    
    mpz_t *e = NULL;

    int found = 0;
    while (!found)
    {
        e = generateLargePrime(keysize, r_state);
        mpz_t gcd_r; mpz_init(gcd_r); mpz_gcd(gcd_r, *e, phi);
        if (mpz_cmp_ui(gcd_r, 1) == 0)
            found = 1;
        mpz_clear(gcd_r);
        if (!found){
            mpz_clear(*e); free(e);
        }
    }

    mpz_t *d = findModInverse(*e, phi);
    
    mpz_t *n1 = malloc(sizeof(mpz_t)); mpz_init_set(*n1, n);
    mpz_t *n2 = malloc(sizeof(mpz_t)); mpz_init_set(*n2, n);
    
    *privk = malloc(sizeof(struct RSA_privKey));
    (*privk)->n = n1;
    (*privk)->d = d;

    *pubk = malloc(sizeof(struct RSA_pubKey));
    (*pubk)->n = n2;
    (*pubk)->e = e;
    
    mpz_clear(phi); mpz_clear(n); mpz_clear(*p); mpz_clear(*q);
    free(q); free(p);
    gmp_randclear(r_state);
}


void RSA_free_private_key(struct RSA_privKey *priv)
{
    mpz_clear(*(priv->n));
    mpz_clear(*(priv->d));
    free(priv->n);
    free(priv->d);
    free(priv);
}


void RSA_free_public_key(struct RSA_pubKey *pub)
{
    mpz_clear(*(pub->n));
    mpz_clear(*(pub->e));
    free(pub->n);
    free(pub->e);
    free(pub);
}


int RSA_pubk_to_file(struct RSA_pubKey *pub, char *path)
{
    if (path == NULL)
        return RSA_ERROR_NULL_PATH;
    if (strcmp(path, "") == 0)
        return RSA_ERROR_EMPTY_PATH;
    FILE *f = fopen(path, "w+");
    if (f == NULL) return RSA_ERROR_CANNOT_OPEN_FD;
    if (mpz_out_raw(f, *(pub->n)) == 0)
        return RSA_ERROR_CANNOT_WRITE_FD;
    if (mpz_out_raw(f, *(pub->e)) == 0)
        return RSA_ERROR_CANNOT_WRITE_FD;
    fclose(f);
    return RSA_OK;
}

int RSA_privk_to_file(struct RSA_privKey *priv, char *path)
{
    if (path == NULL)
        return RSA_ERROR_NULL_PATH;
    if (strcmp(path, "") == 0)
        return RSA_ERROR_EMPTY_PATH;
    FILE *f = fopen(path, "w+");
    if (f == NULL) return RSA_ERROR_CANNOT_OPEN_FD;
    if (mpz_out_raw(f, *(priv->n)) == 0)
        return RSA_ERROR_CANNOT_WRITE_FD;
    if (mpz_out_raw(f, *(priv->d)) == 0)
        return RSA_ERROR_CANNOT_WRITE_FD;
    fclose(f);
    return RSA_OK;
}

struct RSA_pubKey *RSA_pubKey_from_file(char *path)
{
    if (path == NULL)
        return NULL;
    if (strcmp(path, "") == 0)
        return NULL;
    FILE *f = fopen(path, "r");
    
    struct RSA_pubKey *pub = malloc(sizeof(struct RSA_pubKey));

    mpz_t *n = malloc(sizeof(mpz_t)); mpz_init(*n);
    mpz_t *e = malloc(sizeof(mpz_t)); mpz_init(*e);

    mpz_inp_raw(*n, f);
    mpz_inp_raw(*e, f);
    
    pub->n = n;
    pub->e = e;

    fclose(f);
    return pub;
}

struct RSA_privKey *RSA_privKey_from_file(char *path)
{
    if (path == NULL)
        return NULL;
    if (strcmp(path, "") == 0)
        return NULL;
    FILE *f = fopen(path, "r");
    
    struct RSA_privKey *priv = malloc(sizeof(struct RSA_privKey));

    mpz_t *n = malloc(sizeof(mpz_t)); mpz_init(*n);
    mpz_t *d = malloc(sizeof(mpz_t)); mpz_init(*d);

    mpz_inp_raw(*n, f);
    mpz_inp_raw(*d, f);
    
    priv->n = n;
    priv->d = d;

    fclose(f);
    return priv;
}
