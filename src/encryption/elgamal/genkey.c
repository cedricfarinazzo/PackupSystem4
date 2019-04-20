#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <gmp.h>

#include "genkey.h"


mpz_t *ELgenerateLargePrime(unsigned long keysize, gmp_randstate_t r_state)
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


mpz_t *find_primitive_root(mpz_t p, gmp_randstate_t r_state)
{
    mpz_t *g = malloc(sizeof(mpz_t));
    if (mpz_cmp_ui(p, 2) == 0)
    {
        mpz_init_set_ui(*g, 1);
        return g;
    }
    mpz_init(*g);

    mpz_t pr; mpz_init(pr); mpz_sub_ui(pr, p, 3);
    mpz_t p1; mpz_init_set_ui(p1, 2);
    mpz_t p2; mpz_init(p2); mpz_sub_ui(p2, p, 1); mpz_divexact_ui(p2, p2, 2);
    
    mpz_t m1; mpz_init_set(m1, p2);
    mpz_t m2; mpz_init(m2); mpz_sub_ui(m2, p, 1); mpz_divexact(m2, m2, p2);
    
    mpz_t r1, r2; mpz_init(r1); mpz_init(r2);

    while (1) {
        mpz_urandomm(*g, r_state, pr); mpz_add_ui(*g, *g, 2);
        
        mpz_powm_sec(r1, *g, m1, p);
        if (mpz_cmp_ui(r1, 1) != 0)
        {
            mpz_powm_sec(r2, *g, m2, p);
            if (mpz_cmp_ui(r2, 1) != 0)
                break;
        }
    }

    mpz_clear(r1); mpz_clear(r2);
    mpz_clear(m1); mpz_clear(m2);
    mpz_clear(pr); mpz_clear(p1); mpz_clear(p2); 
    return g;
}


void ELGAMAL_generateKey(size_t iNumBits, struct ELGAMAL_privkey **privk, struct ELGAMAL_pubkey **pubk)
{
    gmp_randstate_t r_state, base; 
    gmp_randinit_default(base); mpz_t tmp, ra; mpz_init(tmp); mpz_init(ra); mpz_clear(ra);
    mpz_urandomb(tmp, base, iNumBits);
    gmp_randinit_lc_2exp(r_state, tmp, rand(),128);
    gmp_randclear(base); mpz_clear(tmp);

    mpz_t *p = ELgenerateLargePrime(iNumBits, r_state);

    mpz_t *g = find_primitive_root(*p, r_state);
    mpz_t gbase; mpz_init_set_ui(gbase, 2);
    mpz_powm_sec(*g, *g, gbase, *p); mpz_clear(gbase);

    mpz_t *x = malloc(sizeof(mpz_t)); mpz_init(*x);
    mpz_t xt; mpz_init(xt); mpz_sub_ui(xt, *p, 1); mpz_divexact_ui(xt, xt, 2); mpz_sub_ui(xt, xt, 1);
    mpz_urandomm(*x, r_state, xt); mpz_clear(xt);
    mpz_add_ui(*x, *x, 1);

    mpz_t *h = malloc(sizeof(mpz_t)); mpz_init(*h);
    mpz_powm_sec(*h, *g, *x, *p);

    *pubk = malloc(sizeof(struct ELGAMAL_pubkey));
    *privk = malloc(sizeof(struct ELGAMAL_privkey));
    
    mpz_t *in1 = malloc(sizeof(mpz_t)); mpz_init_set_ui(*in1, iNumBits);
    mpz_t *in2 = malloc(sizeof(mpz_t)); mpz_init_set_ui(*in2, iNumBits);

    mpz_t *p1 = malloc(sizeof(mpz_t)); mpz_init_set(*p1, *p);
    mpz_t *p2 = malloc(sizeof(mpz_t)); mpz_init_set(*p2, *p);
    mpz_clear(*p); free(p);

    mpz_t *g1 = malloc(sizeof(mpz_t)); mpz_init_set(*g1, *g);
    mpz_t *g2 = malloc(sizeof(mpz_t)); mpz_init_set(*g2, *g);
    mpz_clear(*g); free(g);
    
    (*pubk)->iNumBits = in1; (*privk)->iNumBits = in2;
    (*pubk)->p = p1; (*privk)->p = p2;
    (*pubk)->g = g1; (*privk)->g = g2;
    (*pubk)->h = h; 
    (*privk)->x = x;

    gmp_randclear(r_state);
}


void ELGAMAL_privkey_free(struct ELGAMAL_privkey *k)
{
    mpz_clear(*(k->p)); free(k->p);
    mpz_clear(*(k->g)); free(k->g);
    mpz_clear(*(k->x)); free(k->x);
    mpz_clear(*(k->iNumBits)); free(k->iNumBits);
    free(k);
}


void ELGAMAL_pubkey_free(struct ELGAMAL_pubkey *k)
{
    mpz_clear(*(k->p)); free(k->p);
    mpz_clear(*(k->g)); free(k->g);
    mpz_clear(*(k->h)); free(k->h);
    mpz_clear(*(k->iNumBits)); free(k->iNumBits);
    free(k);
}


int ELGAMAL_pubk_to_file(struct ELGAMAL_pubkey *pub, char *path)
{
    if (path == NULL)
        return EL_ERROR_NULL_PATH;
    if (strcmp(path, "") == 0)
        return EL_ERROR_EMPTY_PATH;
    FILE *f = fopen(path, "w+");
    if (f == NULL) return EL_ERROR_CANNOT_OPEN_FD;
    if (mpz_out_raw(f, *(pub->p)) == 0)
        return EL_ERROR_CANNOT_WRITE_FD;
    if (mpz_out_raw(f, *(pub->g)) == 0)
        return EL_ERROR_CANNOT_WRITE_FD;
    if (mpz_out_raw(f, *(pub->h)) == 0)
        return EL_ERROR_CANNOT_WRITE_FD;
    if (mpz_out_raw(f, *(pub->iNumBits)) == 0)
        return EL_ERROR_CANNOT_WRITE_FD;
    fclose(f);
    return EL_OK;
}

int ELGAMAL_privk_to_file(struct ELGAMAL_privkey *priv, char *path)
{
    if (path == NULL)
        return EL_ERROR_NULL_PATH;
    if (strcmp(path, "") == 0)
        return EL_ERROR_EMPTY_PATH;
    FILE *f = fopen(path, "w+");
    if (f == NULL) return EL_ERROR_CANNOT_OPEN_FD;
    if (mpz_out_raw(f, *(priv->p)) == 0)
        return EL_ERROR_CANNOT_WRITE_FD;
    if (mpz_out_raw(f, *(priv->g)) == 0)
        return EL_ERROR_CANNOT_WRITE_FD;
    if (mpz_out_raw(f, *(priv->x)) == 0)
        return EL_ERROR_CANNOT_WRITE_FD;
    if (mpz_out_raw(f, *(priv->iNumBits)) == 0)
        return EL_ERROR_CANNOT_WRITE_FD;
    fclose(f);
    return EL_OK;
}

struct ELGAMAL_pubkey *ELGAMAL_pubKey_from_file(char *path)
{
    if (path == NULL)
        return NULL;
    if (strcmp(path, "") == 0)
        return NULL;
    FILE *f = fopen(path, "r");
    
    struct ELGAMAL_pubkey *pub = malloc(sizeof(struct ELGAMAL_pubkey));

    mpz_t *p = malloc(sizeof(mpz_t)); mpz_init(*p);
    mpz_t *g = malloc(sizeof(mpz_t)); mpz_init(*g);
    mpz_t *h = malloc(sizeof(mpz_t)); mpz_init(*h);
    mpz_t *inb = malloc(sizeof(mpz_t)); mpz_init(*inb);

    mpz_inp_raw(*p, f);
    mpz_inp_raw(*g, f);
    mpz_inp_raw(*h, f);
    mpz_inp_raw(*inb, f);
    
    pub->p = p;
    pub->g = g;
    pub->h = h;
    pub->iNumBits = inb;

    fclose(f);
    return pub;
}

struct ELGAMAL_privkey *ELGAMAL_privKey_from_file(char *path)
{
    if (path == NULL)
        return NULL;
    if (strcmp(path, "") == 0)
        return NULL;
    FILE *f = fopen(path, "r");
    
    struct ELGAMAL_privkey *priv = malloc(sizeof(struct ELGAMAL_privkey));

    mpz_t *p = malloc(sizeof(mpz_t)); mpz_init(*p);
    mpz_t *g = malloc(sizeof(mpz_t)); mpz_init(*g);
    mpz_t *x = malloc(sizeof(mpz_t)); mpz_init(*x);
    mpz_t *inb = malloc(sizeof(mpz_t)); mpz_init(*inb);

    mpz_inp_raw(*p, f);
    mpz_inp_raw(*g, f);
    mpz_inp_raw(*x, f);
    mpz_inp_raw(*inb, f);

    priv->p = p;
    priv->g = g;
    priv->x = x;
    priv->iNumBits = inb;

    fclose(f);
    return priv;
}

