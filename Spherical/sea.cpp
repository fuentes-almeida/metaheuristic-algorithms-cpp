#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "rand.hpp"
#include "memo.hpp"
#include "functions.hpp"
#include "vec.hpp"
#include "sea.hpp"

void InitPopulation(int N, int d, double **X, double xmin, double xmax)
{
    for (int i=0;i<N;i++)
        for (int j=0;j<d;j++)
            X[i][j]=randU(xmin,xmax);
}

void EvaluateFitness(int N, int d, double **X, function fx)
{
    for (int i=0;i<N;i++)
    {
        X[i][d]=fx(d,X[i]);
    }
}

void SelecBestIndividuals(int N, int d, int h, double **X, double **C)
{
    qsort(X,N,sizeof(X[0]),Compare);

    for (int i=0;i<h;i++)
        CopyIndividual(d,X[i],C[i]);
}

void ReplaceIndividuals(int N, int d, double **X, double **Y)
{
    for (int i=0;i<N;i++)
    {
        if (Y[i][d]<X[i][d])
        {
            CopyIndividual(d+1,Y[i],X[i]);
        }
    }
}

void Inversion(int d, double *c, double *x, double *y)
{
    double *aux=RequestVectorDoubleMem(d);

    ///Mutation of the center
    if (randU(0,1)<0.5)
    {
        int k=rand()%d;
        c[k]+=randN(0,1);
    }

    ///Calculate the radius of the hypersphere
    double k=randU(0,2);
    VectorSubstract(d,c,x,aux);
    double norm=VectorNorm(d,aux);
    double radius=k*norm*norm;

    if (norm>0)
    {
        ///Compute the inverse point
        double scalar=(radius*radius)/(norm*norm);
        VectorScalarProduct(d,aux,scalar,aux);
        VectorAdd(d,aux,c,y);

        ///Calculate acceptable regions and identify
        ///when inverse individual is inside or outside
        ///of the acceptable region
        double beta1=radius*exp(-1/radius);
        double beta2=radius*exp( 1/radius);

        VectorSubstract(d,c,y,aux);
        norm=VectorNorm(d,aux);

        if (norm<radius)
        {
            scalar=beta1/norm;
            VectorScalarProduct(d,aux,scalar,aux);
            VectorAdd(d,aux,c,y);
        }
        else if (norm>radius)
        {
            scalar=beta2/norm;
            VectorScalarProduct(d,aux,scalar,aux);
            VectorAdd(d,aux,c,y);
        }
    }
    else
    {
        ///The center of the hypersphere is returned when c=x
        CopyIndividual(d,c,y);
    }
    free(aux);
}

void Reflection(int d, double *c, double *x, double *y)
{
    VectorSubstract(d,c,x,y);

    int jrand=rand()%d;

    for (int j=0;j<d;j++)
        if (j!=jrand)
            y[j]=-y[j];
}

double SphericalEA(int N, int d, int h, function fx, double xmin, double xmax)
{
    int MaxIte=1e4*d/N, k=0, jrand; double fbest=1e10;
    double **X=RequestMatrixDoubleMem(N,d+1);
    double **Y=RequestMatrixDoubleMem(N,d+1);
    double **C=RequestMatrixDoubleMem(h,d);

    InitPopulation(N,d,X,xmin,xmax);

    while (k<MaxIte)
    {
        EvaluateFitness(N,d,X,fx);
        SelecBestIndividuals(N,d,h,X,C);
        fbest=X[0][d];
        //printf("%d %e %lf\n",k,fbest,fbest);

        for (int i=0;i<N;i++)
        {
            do { jrand=rand()%h; } while (jrand==i);

            if (randU(0,1)<0.5)
            {
                Inversion(d,C[jrand],X[i],Y[i]);
            }
            else
            {
                Reflection(d,C[jrand],X[i],Y[i]);
            }
        }
        EvaluateFitness(N,d,Y,fx);
        ReplaceIndividuals(N,d,X,Y);
        k++;
    }

    printf("%d %e %lf\n",k,fbest,fbest);

    //freeMatrizDouble(X);
    freeMatrizDouble(Y);
    freeMatrizDouble(C);

    return fbest;
}
