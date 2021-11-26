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

int compare (const void * a, const void * b)
{
    double x=*(double*)a;
    double y=*(double*)b;
    return x>y;
}

void CopyIndividual(int d, double *src, double *dst)
{
    for (int i=0;i<d;i++)
        dst[i]=src[i];
}

double CalculateDispersion(int N, int dim, double **X)
{
    double *Means=RequestVectorDoubleMem(dim);
    double *StdDevs=RequestVectorDoubleMem(dim);

    for (int i=0;i<N;i++)
        for (int j=0;j<dim;j++)
            Means[j]+=X[i][j];

    for (int i=0;i<dim;i++)
        Means[i]/=N;

    for (int i=0;i<N;i++)
        for (int j=0;j<dim;j++)
            StdDevs[j]+=(X[i][j]-Means[j])*(X[i][j]-Means[j]);

    double mean=0;

    for (int i=0;i<dim;i++)
        mean+=sqrt(StdDevs[i]);
    mean/=dim;

    free(Means);
    free(StdDevs);

    return mean;
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

void UpdateFitness(int N, int d, function fx, double **X, double **Pbest, double **Lbest, double *Elit, int **Top)
{
    for (int i=0;i<N;i++)
    {
        X[i][d]=fx(d,X[i]);
        if (X[i][d]<Pbest[i][d])
        {
            CopyIndividual(d,X[i],Pbest[i]);
            Pbest[i][d]=X[i][d];
        }
        if (X[i][d]<Elit[d])
        {
            CopyIndividual(d,X[i],Elit);
            Elit[d]=X[i][d];
        }
    }

    ///Actualizacion de Lbest de acuerdo a la topologia
    for (int i=0;i<N;i++)
        for (int j=0;j<N;j++)
        {
            if (Top[i][j]==1 && X[j][d]<Lbest[i][d])
            {
                CopyIndividual(d,X[j],Lbest[i]);
                Lbest[i][d]=X[j][d];
            }
        }
}

void InitPopulation(int N, int d, double **X, double **V, double **Pbest, double **Lbest, function fx, double xmin, double xmax)
{
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<d;j++)
        {
            X[i][j]=randU(xmin,xmax);
            V[i][j]=randU(xmin,xmax);
        }
        Lbest[i][d]=1e15;
        Pbest[i][d]=1e15;
    }
}

void UpdateParticles(int N, int d, double **X, double **V, double **Pbest, double **Lbest, double xmin, double xmax, double c1, double c2)
{
    double phi=c1+c2;
    double chi=2/(fabs(2-phi-sqrt(fabs(phi*phi-4*phi))));

    for (int i=0;i<N;i++)
    {
        for (int j=0;j<d;j++)
        {
            V[i][j]=chi*(V[i][j]+c1*randU(0,1)*(Pbest[i][j]-X[i][j])+c2*randU(0,1)*(Lbest[i][j]-X[i][j]));
            X[i][j]=X[i][j]+V[i][j];
        }
    }
}

void GenerateRandomVector(int N, int *RandArray)
{
    for (int i=0;i<N;i++)
        RandArray[i]=i;

    for (int i=0;i<N;i++)
    {
        int r1=rand()%N, r2;
        do {r2=rand()%N;} while (r2==r1);

        int aux=RandArray[r1];
        RandArray[r1]=RandArray[r2];
        RandArray[r2]=aux;
    }
}

void SelectTopology(int N, int **Top, int flag, int k, int kmax, int *RandVec)
{
    ///Conectividad Completa
    if (flag==1)
    {
        for (int i=0;i<N;i++)
            for (int j=0;j<N;j++)
                Top[i][j]=1;

    }
    ///Conectividad de anillo
    if (flag==2)
    {
        for (int i=0;i<N;i++)
        {
            Top[i][(i+1+N)%N]=1;
            Top[i][(i-1+N)%N]=1;
        }
    }
    ///Conectividad propuesta
    if (flag==3)
    {
        int band=(int)(N*k/kmax);
        //printf("%d\n",band);

        for (int i=0;i<N;i++)
        {
            for (int j=0;j<band;j++)
            {
                int aux=RandVec[j];
                Top[i][(i+aux+N)%N]=1;
            }
        }
    }
    if (flag==4)
    {
        for (int i=0;i<N;i++)
            for (int j=0;j<N;j++)
                Top[i][j]=1;

        int band=(int)(N*k/kmax)-1;
        //printf("%d\n",band);

        for (int i=0;i<N;i++)
        {
            for (int j=0;j<band;j++)
            {
                int aux=RandVec[j];
                Top[i][(i+aux+N)%N]=0;
            }
        }
    }
}

double PSO(int N, int d, function fx, double xmin, double xmax, double c1, double c2, int flag)
{
    int MaxIte=1e4*d/N, k=0;

    double **X=RequestMatrixDoubleMem(N,d+1);
    double **V=RequestMatrixDoubleMem(N,d);
    double **Pbest=RequestMatrixDoubleMem(N,d+1);
    double **Lbest=RequestMatrixDoubleMem(N,d+1);
    double *Elit=RequestVectorDoubleMem(d+1);Elit[d]=1e15;
    int **Top=RequestMatrixIntMem(N,N);
    int *RandVec=RequestVectorIntMem(N);
    GenerateRandomVector(N,RandVec);

    InitPopulation(N,d,X,V,Pbest,Lbest,fx,xmin,xmax);

    while(k<MaxIte)
    {
        SelectTopology(N,Top,flag,k+1,MaxIte,RandVec);
        UpdateFitness(N,d,fx,X,Pbest,Lbest,Elit,Top);
        UpdateParticles(N,d,X,V,Pbest,Lbest,xmin,xmax,c1,c2);

        /*for (int i=0;i<N;i++){
            for (int j=0;j<N;j++)
                printf("%d ",Top[i][j]);
            printf("\n");}
            printf("\n");*/

        //double mean=CalculateDispersion(N,d,X);
        //printf("%d %e %lf %lf\n",k,Elit[d],Elit[d],mean);
        k++;
    }
    printf("%d %e %lf\n",k,Elit[d],Elit[d]);

    freeMatrizDouble(X);
    freeMatrizDouble(V);
    freeMatrizDouble(Pbest);
    freeMatrizDouble(Lbest);
    freeMatrizInt(Top);
    free(Elit);

    return Elit[d];
}
