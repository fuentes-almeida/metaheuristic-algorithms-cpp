#include <bitset>
#include <cmath>
#include <iostream>
#include "memo.hpp"
#include "genetic.hpp"

void copyIndividual(bitset<20> *src, bitset<20> *dst, int d)
{
    for (int i=0;i<d;i++)
        for (int j=0;j<BITS;j++)
            dst[i][j]=src[i][j];
}

void EvaluateFitness(int N, int d, bitset<20> **P, double *Fitness, bitset<20> *Elit, function fx, double xmin, double xmax, double *bestResult)
{
    double *decimal=RequestVectorDoubleMem(d);
    double delta = (xmax-xmin)/(pow(2,BITS)-1);
    double mini=999999999;
    int imin=0;

    for (int i=0;i<N;i++)
    {
        for (int j=0;j<d;j++)
        {
            int aux=0;
            for (int k=0;k<BITS;k++)
                aux+=P[i][j][k]*pow(2,k);
            decimal[j]=aux*delta+xmin;
        }

        Fitness[i]=fx(d,decimal);
        if (Fitness[i]<mini)
        {
            mini=Fitness[i];
            imin=i;
        }

    }
    free(decimal);
    *bestResult=Fitness[imin];

    copyIndividual(P[imin],Elit,d);
}

///Inicializa la poblacion a un valor aleatorio entre -100 y 100
void InitPopulation(int N, int d, bitset<20> **P)
{
    for (int i=0;i<N;i++)
        for (int j=0;j<d;j++)
            for (int k=0;k<BITS;k++)
                if (rand()%2==1)
                    P[i][j].set(k,1);

}

void Selection(int N, int d, bitset<20> **P, bitset<20> **P1, double *Fitness)
{
    for (int i=0;i<N;i++)
    {
        int x=rand()%N;
        int y=rand()%N;

        if (Fitness[x]<Fitness[y])
            copyIndividual(P[x],P1[i],d);
        else
            copyIndividual(P[y],P1[i],d);
    }
}


void CrossOver_uniform(bitset<20> *x1, bitset<20> *x2, bitset<20> *y1, bitset<20> *y2, int d)
{
    for (int i=0;i<d;i++)
        for (int j=0;j<BITS;j++)
            if(rand()%2==1)
            {
                y1[i][j]=x1[i][j];
                y2[i][j]=x2[i][j];
            }
            else
            {
                y1[i][j]=x2[i][j];
                y2[i][j]=x1[i][j];
            }
}

void Reproduction(int N, int d, bitset<20> **P1, bitset<20> **P2, double pc)
{
    for (int i=0;i<N;i+=2)
    {
        int x=rand()%N;
        int y=rand()%N;

        double random=1.0*rand()/(double)RAND_MAX;
        if (random<pc)
            CrossOver_uniform(P1[x],P1[y],P2[i],P2[i+1],d);
        else
        {
            copyIndividual(P1[x],P2[i],d);
            copyIndividual(P1[y],P2[i+1],d);
        }
    }
}

void Mutation(int N, int d, bitset<20> **P2)
{
    for (int i=0;i<N;i++)
        for (int j=0;j<d;j++)
        {
            int mutation_pos=rand()%BITS;
            P2[i][j].flip(mutation_pos);
        }
}

void Replacement(int N, int d, bitset<20> **P2, bitset<20> **P3, bitset<20> *Elit)
{
    for (int i=0;i<N;i++)
        copyIndividual(P2[i],P3[i],d);

    int elit_pos=rand()%N;

    copyIndividual(Elit,P3[elit_pos],d);
}

void GeneticAlgorithm(int N, int d, function fx, double xmin, double xmax, double *bestResult)
{
    int Itmax = 10000*d/N;          ///numero de iteraciones
    double pc = 0.85;               ///probabilidad de cruza
    int t=0;

    bitset<20> **P=RequestBitSetMatrix(N,d);
    bitset<20> **P1=RequestBitSetMatrix(N,d);
    bitset<20> **P2=RequestBitSetMatrix(N,d);
    bitset<20> *Elit=RequestBitSetVector(d);
    double *Fitness=RequestVectorDoubleMem(N);

    InitPopulation(N,d,P);

    while (t<Itmax)
    {
        EvaluateFitness(N,d,P,Fitness,Elit,fx,xmin,xmax,bestResult);
        Selection(N,d,P,P1,Fitness);
        Reproduction(N,d,P1,P2,pc);
        Mutation(N,d,P2);
        Replacement(N,d,P2,P,Elit);
        t++;
    }

    EvaluateFitness(N,d,P,Fitness,Elit,fx,xmin,xmax,bestResult);
}

bitset<20> **RequestBitSetMatrix(int data_size1, int data_size2)
{
    bitset<20> **pointer;
    pointer=(bitset<20> **)calloc(data_size1,sizeof(bitset<20>*));
    if (pointer==NULL) return NULL;

    pointer[0]=(bitset<20> *)calloc(data_size1*data_size2,sizeof(bitset<20>));
    if (pointer[0]==NULL) return NULL;

    for (int i=1;i<data_size1;i++)
        pointer[i]=pointer[i-1]+data_size2;

    return pointer;
}

bitset<20> *RequestBitSetVector(int data_size)
{
    bitset<20> *pointer;
    pointer=(bitset<20>*)calloc(data_size,sizeof(bitset<20>));
    return pointer;
}

void freeBitSetMatrix(bitset<20> **pointer)
{
    free(pointer[0]);
    free(pointer);
}
