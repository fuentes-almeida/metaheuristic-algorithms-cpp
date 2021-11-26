#include <iostream>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "memo.hpp"
#include "functions.hpp"
#include "EvoDiff.h"

using namespace std;

int compare (const void * a, const void * b)
{
    double x=*(double*)a;
    double y=*(double*)b;
    return x>y;
}

void CalculateStats(int N, double *vector)
{
    double mini, maxi, mean=0, median, stdDev=0;

    for (int i=0;i<N;i++)
        mean+=vector[i];
    mean=mean/N;

    for (int i=0;i<N;i++)
        stdDev+=(vector[i]-mean)*(vector[i]-mean);
    stdDev=sqrt(stdDev/N);

    qsort(vector,N,sizeof(double),compare);

    mini=vector[0];
    maxi=vector[N-1];

    median=0.5*(vector[N/2-1]+vector[N/2]);

    printf("%e %e %e %e %e\n",mini,maxi,mean,median,stdDev);
}

int main(int argc, char **argv)
{
    srand(time(NULL)+clock());

    int N     = atoi(argv[1]);      ///Tamanio de la poblacion
    int d     = atoi(argv[2]);      ///dimension de la funcion objetivo
    int ref   = atoi(argv[3]);      ///Referencia a la funcion a minimizar
    double CR = atof(argv[4]);
    double F  = atof(argv[5]);
    
    double xmin,xmax;

    switch (ref)
    {
        case 0:     xmin=-5.12;    xmax=5.12;     break;
        case 1:     xmin=-32.768;  xmax=32.768;   break;
        case 2:     xmin=-600;     xmax=600;      break;
        case 3:     xmin=-5.12;    xmax=5.12;     break;
        case 4:     xmin=-2.048;   xmax=2.048;    break;
    }

    function *fx = (function*)malloc(10*sizeof(function));
    fx[0] = Sphere;
    fx[1] = Ackley;
    fx[2] = Griewank;
    fx[3] = Rastrigin;
    fx[4] = Rosenbrock;

    int samples=30;
    double *Data=(double*)calloc(samples,sizeof(double));

    for (int i=0;i<samples;i++)
    Data[i]=DifferentialEvol(N,d,fx[ref],xmin,xmax,CR,F);

    CalculateStats(samples,Data);

    return 0;
}
