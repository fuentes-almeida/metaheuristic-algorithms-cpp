#include <iostream>
#include <cmath>
#include <omp.h>
#include "functions.hpp"
#include "pso.hpp"
#include "memo.hpp"

using namespace std;

int main(int argc, char **argv)
{
    srand(time(NULL)+clock());

    int N    = atoi(argv[1]);      ///Tamanio de la poblacion
    int d    = atoi(argv[2]);      ///dimension de la funcion objetivo
    int ref  = atoi(argv[3]);      ///Referencia a la funcion a minimizar
    int flag = atoi(argv[4]);      ///Referencia a la topologia a usar
    double xmin,xmax,c1=2.05,c2=2.05;

    function *fx = (function*)malloc(10*sizeof(function));
    fx[0] = Sphere;
    fx[1] = Elliptic;
    fx[2] = Schwefel;
    fx[3] = Ackley;
    fx[4] = Rastrigin;
    fx[5] = Griewank;
    fx[6] = Rosenbrock;
    fx[7] = Weierstrass;
    fx[8] = Schaffer;
    fx[9] = Salomon;

    switch (ref)
    {
        case 0:     xmin=-100;      xmax=100;       break;
        case 1:     xmin=-100;      xmax=100;       break;
        case 2:     xmin=-100;      xmax=100;       break;
        case 3:     xmin=-32;       xmax=32;        break;
        case 4:     xmin=-5.12;     xmax=5.12;      break;
        case 5:     xmin=-600;      xmax=600;       break;
        case 6:     xmin=-100;      xmax=100;       break;
        case 7:     xmin=-0.5;      xmax=0.5;       break;
        case 8:     xmin=-0.5;      xmax=0.5;       break;
        case 9:     xmin=-100;      xmax=100;       break;
    }

    int samples=50;

    double *Data=RequestVectorDoubleMem(samples);

    #pragma omp parallel for
        for (int i=0;i<samples;i++)
            Data[i]=PSO(N,d,fx[ref],xmin,xmax,c1,c2,flag);

    CalculateStats(samples,Data);

    return 0;
}
