#include <stdlib.h>
#include "genkey.h"
#include "tools.h"

#include "elgamal.h"

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
