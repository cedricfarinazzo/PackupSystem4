#include <stdlib.h>

#include "rsa.h"

int is_prime(unsigned long n)
{
    if (n%2 == 0)
        return 0;
    int isprime = 1;
    unsigned long i = 3;
    while (isprime && i*i < n) {
        isprime = n % i != 0;
        i+=2;
    }
    return isprime;
}

unsigned long gcd(unsigned long  a, unsigned long b)
{
    while (b != 0)
    {
        unsigned long tmp = a%b;
        a = b;
        b = tmp;
    }
    return a;
}

unsigned long rand_a_b(unsigned long a, unsigned long b)
{
    return rand() % (b - a) + a;
}

unsigned int find_coprime(unsigned long phi, unsigned long max)
{
    unsigned int b = rand_a_b(0, b);
    while (gcd(b, phi) != 1)
    {
        b = rand_a_b(0, b);
    }
    return b;
}

void double_assign(unsigned long *a, unsigned long *b, unsigned long c, unsigned long d)
{
    *a = c;
    *b = d;
}

void extendedEuclideanAlgorithm(unsigned long a, unsigned long b)
{
    unsigned long s = 0;
    unsigned long t = 0;
    unsigned long r = b;

    unsigned long old_r = 1;
    unsigned long old_s = 0;
    unsigned long old_t = a;

    while (r != 0)
    {
        unsigned long quotient = old_r / r;
        double_assign(&old_r, &r, r, old_r - quotient * r);
        double_assign(&old_s, &s, s, old_s - quotient * s);
        double_assign(&old_t, &t, t, old_t - quotient * t);
    }

    // return 

}



void RSA_genKey(unsigned long p, unsigned long q)
{
    unsigned long n = p*q;
    unsigned long phi_n = (p-1) * (q-1);
    
    unsigned long e = find_coprime(phi_n, phi_n-1);

}
