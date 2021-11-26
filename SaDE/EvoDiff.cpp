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
#include "EvoDiff.h"

void CopyIndividual(int d, double *src, double *dst)
{
    for (int i=0;i<d;i++)
        dst[i]=src[i];
}

void InitPopulation(int N, int d, double xmin, double xmax, double **P, function fx, double *Fitness, int *Ibest, double *Fbest)
{
    double rank=xmax-xmin, mini=1e15;
    int imin;

    for (int i=0;i<N;i++)
    {
        for (int j=0;j<d;j++)
            P[i][j]=rank*randU()+xmin;

        Fitness[i]=fx(d,P[i]);
        if (Fitness[i]<mini)
        {
            mini=Fitness[i];
            imin=i;
        }
    }
    *Ibest=imin;
    *Fbest=Fitness[imin];
}

void Mutation(int N, int d, double xmin, double xmax, double **X, double **V, int Ibest, double Prob, int *MutEquation)
{
    int r1,r2,r3;

    double Factor;

    for (int i=0;i<N;i++)
    {
        do { r1=rand()%N; } while (r1==i);
        do { r2=rand()%N; } while (r2==i || r2==r1);
        do { r3=rand()%N; } while (r3==i || r3==r1 || r3==r2);

        Factor=randN(0.5,0.3);

        if (randU()<Prob)
        {
            for (int j=0;j<d;j++)
            {
                double aux=X[r1][j]+Factor*(X[r2][j]-X[r3][j]);
                while (aux>xmax || aux<xmin)
                {
                    if (aux>xmax)
                        aux-=(aux-xmax);
                    if (aux<xmin)
                        aux+=(xmin-aux);
                }
                V[i][j]=aux;
            }
            MutEquation[i]=1;
        }
        else
        {
            for (int j=0;j<d;j++)
            {
                double aux=X[i][j]+Factor*((X[Ibest][j]-X[i][j])+(X[r1][j]-X[r2][j]));
                while (aux>xmax || aux<xmin)
                {
                    if (aux>xmax)
                        aux-=(aux-xmax);
                    if (aux<xmin)
                        aux+=(xmin-aux);
                }
                V[i][j]=aux;
            }
            MutEquation[i]=3;
        }
    }
}

void Crossover(int N, int d, double **X, double **V, double **U, int jrand, int *MutEquation, int* counter, double *CRis)
{
    for (int i=0;i<N;i++)
    {
        if (randU()<=CRis[i] || i==jrand)
            CopyIndividual(d,V[i],U[i]);
        else
        {
            CopyIndividual(d,X[i],U[i]);
            if (MutEquation[i]==1)
                counter[2]++;
            if (MutEquation[i]==3)
                counter[3]++;
            MutEquation[i]=0;
        }
    }
}

void Selection(int N, int d, double **X, double **U, function fx, double *Fitness, int *Ibest, double *Fbest,int *MutEquation, int *counter, double *CRis, vector<double> *CRrec)
{
    for (int i=0;i<N;i++)
    {
        double fitness_new=fx(d,U[i]);

        if (fitness_new < Fitness[i])
        {
            CopyIndividual(d,U[i],X[i]);
            Fitness[i]=fitness_new;

            if (fitness_new < *Fbest)
            {
                *Fbest=fitness_new;
                *Ibest=i;
            }

            if (MutEquation[i]==1)
                counter[0]++;
            if (MutEquation[i]==3)
                counter[1]++;
            if (MutEquation[i]!=0)
                CRrec->push_back(CRis[i]);
        }
        else
        {
            if (MutEquation[i]==1)
                counter[2]++;
            if (MutEquation[i]==3)
                counter[3]++;
        }
    }
}

double updateProb(int *counter)
{
    double P_new=(double)(counter[0]*(counter[1]+counter[3]))/(counter[1]*(counter[0]+counter[2])+counter[0]*(counter[1]+counter[3]));

    for (int i=0;i<4;i++)
        counter[i]=0;
    printf("Prob updated = %lf\n",P_new);
    return P_new;
}

double updateCR(vector <double> *CRrec)
{
    double mean=0;
    int num=CRrec->size();

      while (!CRrec->empty())
      {
        mean+=CRrec->back();
        CRrec->pop_back();
      }
    mean=mean/num;

    printf("CRm updated = %lf\n",mean);
    return mean;
}

double DifferentialEvol(int N, int d, function fx, double xmin, double xmax)
{
    int MaxIte=1e5*d/N, k=0, Ibest;
    double Fbest=1000, Prob=0.5, CRm=0.5;

    double **X=RequestMatrixDoubleMem(N,d);
    double **V=RequestMatrixDoubleMem(N,d);
    double **U=RequestMatrixDoubleMem(N,d);
    double *Fitness=RequestVectorDoubleMem(N);
    int *MutEquation=RequestVectorIntMem(N);
    double *CRis=RequestVectorDoubleMem(N);
    int *counter=RequestVectorIntMem(4);
    vector <double> CRrec;

    for (int i=0;i<N;i++)
        CRis[i]=randN(CRm,0.1);

    InitPopulation(N,d,xmin,xmax,X,fx,Fitness,&Ibest,&Fbest);

    while (Fbest>1e-8 && k<MaxIte)
    {
        Mutation(N,d,xmin,xmax,X,V,Ibest,Prob,MutEquation);

        int jrand=rand()%N;
        Crossover(N,d,X,V,U,jrand,MutEquation,counter,CRis);

        Selection(N,d,X,U,fx,Fitness,&Ibest,&Fbest,MutEquation,counter,CRis,&CRrec);

        //printf("%d %lf %e %d %d %d %d\n",k, Fbest,Fbest,counter[0],counter[1],counter[2],counter[3]);
        printf("%lf\n",randU());
        k++;
        if (k%5==0){
            for (int i=0;i<N;i++)
                CRis[i]=randN(CRm,0.1);}
        if (k%25==0)
            CRm=updateCR(&CRrec);
        if (k%50==0)
            Prob=updateProb(counter);
    }
    printf("%d %lf %e\n",k, Fbest,Fbest);

    freeMatrizDouble(X);
    freeMatrizDouble(V);
    freeMatrizDouble(U);
    free(Fitness);
    free(MutEquation);
    free(CRis);
    free(counter);

    return Fbest;
}

