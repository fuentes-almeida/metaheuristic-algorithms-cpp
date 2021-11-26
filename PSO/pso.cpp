#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "rand.hpp"
#include "memo.hpp"
#include "functions.hpp"
#include "pso.hpp"

void CopyIndividual(int d, double *src, double *dst)
{
    for (int i=0;i<d;i++)
        dst[i]=src[i];
}

void InitPopulation(int N, int d, double **X, double **V, double **P, function fx, double xmin, double xmax, double *Best)
{
    double fmin=1e15; int imin;

    for (int i=0;i<N;i++)
    {
        for (int j=0;j<d;j++)
        {
            X[i][j]=randU(xmin,xmax);
            P[i][j]=X[i][j];
            V[i][j]=randU(xmin,xmax);
        }

        X[i][d]=fx(d,X[i]);
        P[i][d]=X[i][d];
        if (X[i][d]<fmin)
        {
            fmin=X[i][d];
            imin=i;
        }
    }
    CopyIndividual(d,X[imin],Best);
    Best[d]=fmin;
}

void UpdateParticles(int N, int d, double **X, double **V, double **P, double *Best, double xmin, double xmax, double c1, double c2)
{
    double aux;
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<d;j++)
        {
            aux=V[i][j]+c1*randU(0,1)*(P[i][j]-X[i][j])+c2*randU(0,1)*(Best[j]-X[i][j]);
                while (aux>xmax || aux<xmin)
                {
                    if (aux>xmax)
                        aux-=(aux-xmax);
                    if (aux<xmin)
                        aux+=(xmin-aux);
                }
            V[i][j]=aux;

            aux=X[i][j]+V[i][j];
                while (aux>xmax || aux<xmin)
                {
                    if (aux>xmax)
                        aux-=(aux-xmax);
                    if (aux<xmin)
                        aux+=(xmin-aux);
                }
            X[i][j]=aux;
        }
    }
}

void UpdateFitness(int N, int d, function fx, double **X, double **P, double *Best)
{
    for (int i=0;i<N;i++)
    {
        X[i][d]=fx(d,X[i]);
        if (X[i][d]<P[i][d])
        {
            CopyIndividual(d,X[i],P[i]);
            P[i][d]=X[i][d];
        }
        if (X[i][d]<Best[d])
        {
            CopyIndividual(d,X[i],Best);
            Best[d]=X[i][d];
        }
    }
}

double PSO(int N, int d, function fx, double xmin, double xmax, double c1, double c2)
{
    int MaxIte=1e5*d/N, k=0;
    double Fbest=1000;

    double **X=RequestMatrixDoubleMem(N,d+1);
    double **V=RequestMatrixDoubleMem(N,d);
    double **P=RequestMatrixDoubleMem(N,d+1);
    double *Best=RequestVectorDoubleMem(d+1);

    InitPopulation(N,d,X,V,P,fx,xmin,xmax,Best);

    while(k<MaxIte)
    {
        UpdateParticles(N,d,X,V,P,Best,xmin,xmax,c1,c2);
        UpdateFitness(N,d,fx,X,P,Best);


        k++;
    }
    printf("%lf %lf %d %lf\n",c1,c2,k,Best[d]);

    freeMatrizDouble(X);
    freeMatrizDouble(V);
    freeMatrizDouble(P);
    free(Best);

    return Fbest;
}
