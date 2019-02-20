#include <stdlib.h>
#include <gmp.h>
#include "tools.h"
#include "genkey.h"


mpz_t *prime_founder(mpz_t a, mpz_t debut)
{
    mpz_t *r = malloc(sizeof(mpz_t));
    mpz_init(*r);

    while (mpz_cmp(debut, a) <= 0)
    {
        mpz_t pg;
        mpz_init(pg);
        mpz_gcd(pg, a, debut);
        if (mpz_cmp_ui(pg, 1) == 0)
        {
            mpz_clear(pg);
            mpz_set(*r, debut);
            return r;
        }
        mpz_add_ui(debut, debut, 1);
        mpz_clear(pg);
    }
    
    mpz_set_si(*r, -1);
    return r;
}

mpz_t *mod_founder(mpz_t e, mpz_t phi, mpz_t d)
{
    mpz_t *r = malloc(sizeof(mpz_t));
    mpz_init(*r);
    while (mpz_cmp(d, phi) <= 0)
    {

        mpz_t pg;
        mpz_init(pg);
        mpz_mul(pg, e, d);
        mpz_mod(pg, pg, phi);
        if (mpz_cmp_ui(pg, 1) == 0)
        {
            mpz_clear(pg);
            mpz_set(*r, d);
            return r;
        }

        mpz_add_ui(d, d, 1);
        mpz_clear(pg);
    }

    mpz_set_si(*r, -1);
    return r;
}

struct RSA_publickey *RSA_gen_public_key(mpz_t p, mpz_t q)
{
    mpz_t n;
    mpz_init(n);
    mpz_mul(n, p, q);

    mpz_t p1;
    mpz_init(p1);
    mpz_set(p1, p);
    mpz_sub_ui(p1, p1, 1);

    mpz_t q1;
    mpz_init(q1);
    mpz_set(q1, q);
    mpz_sub_ui(q1, q1, 1);

    mpz_t phi;
    mpz_init(phi);
    mpz_mul(phi, p1, q1);

    mpz_t mi; mpz_init(mi);
    min(p, q, mi);

    mpz_t *e = prime_founder(phi, mi);

    mpz_clear(mi);
    mpz_clear(p1);
    mpz_clear(q1);

    mpz_t *ns = malloc(sizeof(mpz_t));
    mpz_init(*ns);
    mpz_set(*ns, n);

    struct RSA_publickey *public = malloc(sizeof(struct RSA_publickey));
    public->e = e;
    public->n = ns;

    mpz_clear(phi);
    mpz_clear(n);

    return public;
}

void RSA_free_public_key(struct RSA_publickey *pub)
{
    mpz_clear(*(pub->n));
    mpz_clear(*(pub->e));
    free(pub->n);
    free(pub->e);
    free(pub);
}

struct RSA_privatekey *RSA_gen_private_key(mpz_t p, mpz_t q, struct RSA_publickey *public)
{

    mpz_t n;
    mpz_init(n);
    mpz_mul(n, p, q);

    mpz_t p1;
    mpz_init(p1);
    mpz_set(p1, p);
    mpz_sub_ui(p1, p1, 1);

    mpz_t q1;
    mpz_init(q1);
    mpz_set(q1, q);
    mpz_sub_ui(q1, q1, 1);

    mpz_t phi;
    mpz_init(phi);
    mpz_mul(phi, p1, q1);

    mpz_t mi; mpz_init(mi);
    min(p, q, mi);

    mpz_t *d = mod_founder(*(public->e), phi, mi);

    mpz_clear(mi);
    mpz_clear(p1);
    mpz_clear(q1);

    mpz_t *ns = malloc(sizeof(mpz_t));
    mpz_init(*ns);
    mpz_set(*ns, n);

    mpz_clear(phi);
    mpz_clear(n);

    struct RSA_privatekey *private = malloc(sizeof(struct RSA_privatekey));
    private->d = d;
    private->n = ns;
    return private;
}

void RSA_free_private_key(struct RSA_privatekey *priv)
{
    mpz_clear(*(priv->n));
    mpz_clear(*(priv->d));
    free(priv->n);
    free(priv->d);
    free(priv);
}
