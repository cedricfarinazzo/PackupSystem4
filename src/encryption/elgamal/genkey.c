#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "tools.h"

#include "genkey.h"


void ELGAMAL_generateKey(size_t size, struct ELGAMAL_privkey **privk, struct ELGAMAL_pubkey **pubk)
{
        long long max_size = (long long)pow(2, (double) size+1);
	long long p, d, e1, e2;
        do {
		do {
			p = rand() + 256;
                        p = p % max_size;
                } while (p % 2 == 0);

	} while (!PrimarityTest(2, p));

	e1 = rand() % p;
	do {
		d = rand() % (p - 2) + 1; // 1 <= d <= p-2
	} while (gcd(d, p) != 1);

	e2 = FindT(e1, d, p);

        *privk = malloc(sizeof(struct ELGAMAL_privkey));
        *pubk = malloc(sizeof(struct ELGAMAL_pubkey));

        (*privk)->p = p;
        (*privk)->d = d;
        
        (*pubk)->p = p;
        (*pubk)->e1 = e1;
        (*pubk)->e2 = e2;

}


void ELGAMAL_privkey_free(struct ELGAMAL_privkey *k)
{
    free(k);
}


void ELGAMAL_pubkey_free(struct ELGAMAL_pubkey *k)
{
    free(k);
}

int ELGAMAL_pubk_to_stream(struct ELGAMAL_pubkey *pub, FILE *f)
{
    if (f == NULL)
        return EL_ERROR_NULL_PATH;
    fwrite(&(pub->p), 1, sizeof(long long), f);
    fwrite(&(pub->e1), 1, sizeof(long long), f);
    fwrite(&(pub->e2), 1, sizeof(long long), f);
    return EL_OK;
}

int ELGAMAL_privk_to_stream(struct ELGAMAL_privkey *priv, FILE *f)
{
    if (f == NULL)
        return EL_ERROR_NULL_PATH;
    fwrite(&(priv->p), 1, sizeof(long long), f);
    fwrite(&(priv->d), 1, sizeof(long long), f);
    return EL_OK;
}

int ELGAMAL_pubk_to_file(struct ELGAMAL_pubkey *pub, char *path)
{
    if (path == NULL)
        return EL_ERROR_NULL_PATH;
    if (strcmp(path, "") == 0)
        return EL_ERROR_EMPTY_PATH;
    FILE *f = fopen(path, "w+");
    if (f == NULL) return EL_ERROR_CANNOT_OPEN_FD;
    
    ELGAMAL_pubk_to_stream(pub, f);

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
    
    ELGAMAL_privk_to_stream(priv, f);

    fclose(f);
    return EL_OK;
}

int ELGAMAL_pubk_from_stream(struct ELGAMAL_pubkey *pub, FILE *f)
{
    if (f == NULL)
        return EL_ERROR_NULL_PATH;
    fread(&(pub->p), 1, sizeof(long long), f);
    fread(&(pub->e1), 1, sizeof(long long), f);
    fread(&(pub->e2), 1, sizeof(long long), f);
    return EL_OK;
}

int ELGAMAL_privk_from_stream(struct ELGAMAL_privkey *priv, FILE *f)
{
    if (f == NULL)
        return EL_ERROR_NULL_PATH;
    fread(&(priv->p), 1, sizeof(long long), f);
    fread(&(priv->d), 1, sizeof(long long), f);
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

    ELGAMAL_pubk_from_stream(pub, f);

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

    ELGAMAL_privk_from_stream(priv, f);

    fclose(f);
    return priv;
}

