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
    double range=xmax-xmin, mini=1e15;
    int imin;

    for (int i=0;i<N;i++)
    {
        for (int j=0;j<d;j++)
            P[i][j]=range*randU()+xmin;

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

void Mutation(int N, int d, double xmin, double xmax, double **X, double **V, int Ibest, double Prob, double fp, int *MutEquation, int *Distr)
{
    int r1,r2,r3;

    double Factor;

    for (int i=0;i<N;i++)
    {
        do { r1=rand()%N; } while (r1==i);
        do { r2=rand()%N; } while (r2==i || r2==r1);
        do { r3=rand()%N; } while (r3==i || r3==r1 || r3==r2);

        if (randU()<fp)
        {
            Factor=randN(0.5,0.3);
            Distr[i]=1;
        }
        else
        {
            Factor=randC(0.5,1.0);
            Distr[i]=2;
        }
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

void Crossover(int N, int d, double **X, double **V, double **U, int jrand, int *MutEquation, int* Ecounter, int *Distr, int *Fcounter, double *CRis)
{
    for (int i=0;i<N;i++)
    {
        if (randU()<=CRis[i] || i==jrand)
            CopyIndividual(d,V[i],U[i]);
        else
        {
            CopyIndividual(d,X[i],U[i]);
            if (MutEquation[i]==1)
                Ecounter[2]++;
            if (MutEquation[i]==3)
                Ecounter[3]++;
            MutEquation[i]=0;

            if (Distr[i]==1)
                Fcounter[2]++;
            if (Distr[i]==2)
                Fcounter[3]++;
            Distr[i]=0;
        }
    }
}

void Selection(int N, int d, double **X, double **U, function fx, double *Fitness, int *Ibest, double *Fbest,int *MutEquation, int *Ecounter, int *Distr, int *Fcounter, double *CRis, vector<double> *CRrec, vector <double> *deltaF)
{
    for (int i=0;i<N;i++)
    {
        double fitness_new=fx(d,U[i]);

        if (fitness_new < Fitness[i])
        {
            CopyIndividual(d,U[i],X[i]);
            if (fitness_new < *Fbest)
            {
                *Fbest=fitness_new;
                *Ibest=i;
            }

            if (MutEquation[i]==1)
                Ecounter[0]++;
            if (MutEquation[i]==3)
                Ecounter[1]++;
            if (Distr[i]==1)
                Fcounter[0]++;
            if (Distr[i]==2)
                Fcounter[1]++;
            if (MutEquation[i]!=0)
            {
                CRrec->push_back(CRis[i]);
                deltaF->push_back(Fitness[i]-fitness_new);
            }
            Fitness[i]=fitness_new;
        }
        else
        {
            if (MutEquation[i]==1)
                Ecounter[2]++;
            if (MutEquation[i]==3)
                Ecounter[3]++;
            if (Distr[i]==1)
                Fcounter[2]++;
            if (Distr[i]==2)
                Fcounter[3]++;
        }
    }
}

double updateProb(int *counter)
{
    double P_new=(double)(counter[0]*(counter[1]+counter[3]))/(counter[1]*(counter[0]+counter[2])+counter[0]*(counter[1]+counter[3]));

    for (int i=0;i<4;i++)
        counter[i]=0;
    //printf("Prob updated = %lf\n",P_new);
    return P_new;
}

double updateCR(vector <double> *CRrec, vector <double> *deltaF)
{
    double mean=0, sum=0, wk;
    int num=CRrec->size();

    for (int i=0;i<num;i++)
        sum+=deltaF->at(i);

    for (int k=0;k<num;k++)
    {
        wk=deltaF->at(k)/sum;
        mean+=wk*CRrec->at(k);
    }

    while(!CRrec->empty())
    {
        CRrec->pop_back();
        deltaF->pop_back();
    }

    //printf("CRm updated = %lf\n",mean);
    return mean;
}

double DifferentialEvol(int N, int d, function fx, double xmin, double xmax)
{
    int MaxIte=1e5*d/N, k=0, Ibest;
    double Fbest=1000, Prob=0.5, CRm=0.5, fp=0.5;

    double **X=RequestMatrixDoubleMem(N,d);
    double **V=RequestMatrixDoubleMem(N,d);
    double **U=RequestMatrixDoubleMem(N,d);
    double *Fitness=RequestVectorDoubleMem(N);

    int *MutEquation=RequestVectorIntMem(N);
    int *Distr=RequestVectorIntMem(N);
    int *Ecounter=RequestVectorIntMem(4);
    int *Fcounter=RequestVectorIntMem(4);
    double *CRis=RequestVectorDoubleMem(N);
    vector <double> deltaF;
    vector <double> CRrec;

    for (int i=0;i<N;i++)
        CRis[i]=randN(CRm,0.1);

    InitPopulation(N,d,xmin,xmax,X,fx,Fitness,&Ibest,&Fbest);

    while (Fbest>1e-8 && k<MaxIte)
    {
        Mutation(N,d,xmin,xmax,X,V,Ibest,Prob,fp,MutEquation,Distr);

        Crossover(N,d,X,V,U,rand()%N,MutEquation,Ecounter,Distr,Fcounter,CRis);

        Selection(N,d,X,U,fx,Fitness,&Ibest,&Fbest,MutEquation,Ecounter,Distr,Fcounter,CRis,&CRrec,&deltaF);

        //printf("%d %lf %e %d %d %d %d %d %d %d %d\n",k, Fbest,Fbest,Ecounter[0],Ecounter[1],Ecounter[2],Ecounter[3],Fcounter[0],Fcounter[1],Fcounter[2],Fcounter[3]);

        k++;

        if (k%25==0)
            CRm=updateCR(&CRrec,&deltaF);

        if (k%5==0){
            for (int i=0;i<N;i++)
                CRis[i]=randN(CRm,0.1);}

        if (k%50==0)
        {
            Prob=updateProb(Ecounter);
            fp=updateProb(Fcounter);
        }
    }
    printf("%d %lf %e\n",k, Fbest,Fbest);

    freeMatrizDouble(X);
    freeMatrizDouble(V);
    freeMatrizDouble(U);
    free(Fitness);
    free(MutEquation);
    free(Distr);
    free(Ecounter);
    free(Fcounter);
    free(CRis);

    return Fbest;
}

