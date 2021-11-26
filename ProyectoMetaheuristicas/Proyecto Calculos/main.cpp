#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <omp.h>
#include "memo.hpp"
#include "function.hpp"

using namespace std;

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    int MaxIte = atoi(argv[2]);

    srand(time(NULL)+clock());

    FILE *input=fopen("input.dat","r");
    if (!input){printf("Imposible abrir archivo");return 0;}

    int num;
    fscanf(input,"%d",&num);
    double **TargetPts=RequestMatrixDoubleMem(2,num);

    for (int i=0;i<num;i++)
        fscanf(input,"%lf %lf",&TargetPts[0][i],&TargetPts[1][i]);
    fclose(input);

#pragma omp parallel for
    for (int i=0;i<10;i++)
    {
        MemeticAlgorithm(N,num,TargetPts,MaxIte);
    }

    return 0;
}
