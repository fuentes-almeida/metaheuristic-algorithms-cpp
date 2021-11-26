#include <iostream>
#include "functions.hpp"
#include "pso.hpp"
using namespace std;

int main(int argc, char **argv)
{
    srand(time(NULL)+clock());

    int N     = atoi(argv[1]);      ///Tamanio de la poblacion
    int d     = atoi(argv[2]);      ///dimension de la funcion objetivo
    int ref   = atoi(argv[3]);      ///Referencia a la funcion a minimizar
    double c1 = atof(argv[4]);
    double c2 = atof(argv[5]);
    double xmin,xmax;

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

for (ref=0;ref<10;ref++)
{
    switch (ref)
    {
        case 0:     xmin=-100;      xmax=100;       c1=0.5;c2=0.1;break;
        case 1:     xmin=-100;      xmax=100;       c1=0.8;c2=0.1;break;
        case 2:     xmin=-100;      xmax=100;       c1=0.9;c2=0.1;break;
        case 3:     xmin=-32;       xmax=32;        c1=0.7;c2=0.3;break;
        case 4:     xmin=-5.12;     xmax=5.12;      c1=0.6;c2=0.4;break;
        case 5:     xmin=-600;      xmax=600;       c1=0.7;c2=0.1;break;
        case 6:     xmin=-100;      xmax=100;       c1=0.6;c2=0.1;break;
        case 7:     xmin=-0.5;      xmax=0.5;       c1=0.5;c2=0.1;break;
        case 8:     xmin=-0.5;      xmax=0.5;       c1=0.5;c2=0.1;break;
        case 9:     xmin=-100;      xmax=100;       c1=0.7;c2=0.1;break;
    }

    printf("%d ",ref);
    PSO(N,d,fx[ref],xmin,xmax,c1,c2);

}

    return 0;
}
