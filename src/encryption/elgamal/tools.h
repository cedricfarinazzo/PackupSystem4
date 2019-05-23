#ifndef _SRC_ENCRYPTION_ELGAMAL_TOOLS_H_
#define _SRC_ENCRYPTION_ELGAMAL_TOOLS_H_

#include <stdlib.h>
#include <math.h>

long long gcd(long long a, long long b);

void FastExponention(long long bit, long long n, long long* y, long long* a);

long long FindT(long long a, long long m, long long n);

long long PrimarityTest(long long a, long long i);

#endif /* _SRC_ENCRYPTION_ELGAMAL_TOOLS_H_ */
