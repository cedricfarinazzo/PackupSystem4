#include <stdlib.h>
#include <math.h>

#include "tools.h"

long long gcd(long long a, long long b)
{
	long long q, r1, r2, r;

	if (a > b) {
		r1 = a;
		r2 = b;
	}
	else {
		r1 = b;
		r2 = a;
	}

	while (r2 > 0) {
		q = r1 / r2;
		r = r1 - q * r2;
		r1 = r2;
		r2 = r;
	}

	return r1;
}

void FastExponention(long long bit, long long n, long long* y, long long* a)
{
	if (bit == 1)
		*y = (*y * (*a)) % n;

	*a = (*a) * (*a) % n;
}

long long FindT(long long a, long long m, long long n)
{
	long long r;
	long long y = 1;

	while (m > 0)
	{
		r = m % 2;
		FastExponention(r, n, &y, &a);
		m = m / 2;
	}

	return y;
}

long long PrimarityTest(long long a, long long i)
{
	long long n = i - 1;
	long long k = 0;
	long long m, T;

	while (n % 2 == 0) {
		k++;
		n = n / 2;
	}

	m = n;
	T = FindT(a, m, i);
	if (T == 1 || T == i - 1)
		return 1;

	for (long long j = 0; j < k; j++) {
		T = FindT(T, 2, i);
		if (T == 1)
			return 0;
		if (T == i - 1)
			return 1;
	}

	return 0;
}
