#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "vec.hpp"

double VectorNorm(int d, double *v)
{
    double norm=0;
    for (int i=0;i<d;i++)
        norm+=v[i]*v[i];

    return sqrt(norm);
}

double SquareDistance(int d, double *v1, double *v2)
{
    double dist=0;
    for (int i=0;i<d;i++)
        dist+=(v1[i]-v2[i])*(v1[i]-v2[i]);

    return dist;
}

void VectorScalarProduct(int d, double *v, double scalar, double *result)
{
    for (int i=0;i<d;i++)
        result[i]=v[i]*scalar;
}

void VectorAdd(int d, double *v1, double *v2, double *result)
{
    for (int i=0;i<d;i++)
        result[i]=v1[i]+v2[i];
}

void VectorSubstract(int d, double *v1, double *v2, double *result)
{
    for (int i=0;i<d;i++)
        result[i]=v1[i]-v2[i];
}

void CopyIndividual(int d, double *src, double *dst)
{
    for (int i=0;i<d;i++)
        dst[i]=src[i];
}

int Compare (const void * a, const void * b)
{
    double *x=*(double**)a;
    double *y=*(double**)b;

    if (x[dim]<y[dim]) return-1;
    if (x[dim]>y[dim]) return 1;
    else return 0;
}
