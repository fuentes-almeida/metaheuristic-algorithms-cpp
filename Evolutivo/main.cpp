#include <iostream>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "memo.hpp"
#include "functions.hpp"
#include "genetic.hpp"

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

    //cout<<mini<<" "<<maxi<<" "<<mean<<" "<<median<<" "<<stdDev<<" "<<endl;

}

int main(int argc, char **argv)
{
    srand(time(NULL)+clock());

    int N     = atoi(argv[1]);      ///Tamanio de la poblacion
    int d     = atoi(argv[2]);      ///dimension de la funcion objetivo
    int ref   = atoi(argv[3]);      ///Referencia a la funcion a minimizar
    double xmin = atoi(argv[4]);
    double xmax = atoi(argv[5]);

    function *fx = (function*)malloc(5*sizeof(function));
    fx[0]=Sphere;
    fx[1]=Ackley;
    fx[2]=Griewank;
    fx[3]=Rastrigin;
    fx[4]=Rosenbrock;

    int samples=10;

    double *bestResults=RequestVectorDoubleMem(samples);
    for (int i=0;i<samples;i++)
    GeneticAlgorithm(N,d,fx[ref],xmin,xmax,&bestResults[i]);

    CalculateStats(samples,bestResults);

    return 0;
}
