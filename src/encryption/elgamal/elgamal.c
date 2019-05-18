#include <stdio.h>
#include <stdlib.h>
#include "genkey.h"
#include "tools.h"

#include "elgamal.h"

int toascii(int a)
{
    return (int)((unsigned char) a);
}

void EL_encryption_single(int c, long long *c1, long long *c2, struct ELGAMAL_pubkey *pub)
{
	long long r;
	do {
		r = rand() % (pub->p - 1) + 1; // 1 < r < p
	}
	while (gcd(r, pub->p) != 1);

	*c1 = FindT(pub->e1, r, pub->p);
	*c2 = FindT(pub->e2, r, pub->p) * c % pub->p;
}

long long EL_decryption_single(long long c1, long long c2, struct ELGAMAL_privkey *priv)
{
	return c2 * FindT(c1, priv->p - 1 - priv->d, priv->p) % priv->p;
}

void EL_encryption_stream(FILE *in, FILE *out, struct ELGAMAL_pubkey *pub)
{
    if (in == NULL || out == NULL) return;

    char c;
    long long c1, c2;
    while (fread(&c, sizeof(char), 1, in) != 0)
    {
        EL_encryption_single(toascii(c), &c1, &c2, pub);
        fwrite(&c1, sizeof(long long), 1, out);
        fwrite(&c2, sizeof(long long), 1, out);
    }
}

void EL_decryption_stream(FILE *in, FILE *out, struct ELGAMAL_privkey *priv)
{
    if (in == NULL || out == NULL) return;

    char c;
    long long c1, c2;
    while (1)
    {
        int ret = fread(&c1, sizeof(long long), 1, in);
        fread(&c2, sizeof(long long), 1, in);
        if (ret <= 0)
            break;
        c = (char)EL_decryption_single( c1, c2, priv);
        fprintf(out, "%c", c);
    }
}

void EL_encryption_file(char *in, char *out, struct ELGAMAL_pubkey *pub)
{
    if (in == NULL || out == NULL) return;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "wb+");
    EL_encryption_stream(fin, fout, pub);
    fclose(fout);
    fclose(fin);
}

void EL_decryption_file(char *in, char *out, struct ELGAMAL_privkey *priv)
{
    if (in == NULL || out == NULL) return;
    FILE *fin = fopen(in, "rb");
    FILE *fout = fopen(out, "w+");
    EL_decryption_stream(fin, fout, priv);
    fclose(fout);
    fclose(fin);
}

void EL_encryption_fd(int in, int out, struct ELGAMAL_pubkey *pub)
{
    if (in == 0 || out == 0) return;
    FILE *fin = fdopen(in, "r");
    FILE *fout = fdopen(out, "wb+");
    EL_encryption_stream(fin, fout, pub);
    fclose(fout);
    fclose(fin);
}

void EL_decryption_fd(int in, int out, struct ELGAMAL_privkey *priv)
{
    if (in == 0 || out == 0) return;
    FILE *fin = fdopen(in, "rb");
    FILE *fout = fdopen(out, "w+");
    EL_decryption_stream(fin, fout, priv);
    fclose(fout);
    fclose(fin);
}
