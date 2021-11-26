#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shell.hpp"
#include "function.hpp"
#define PI 3.141592
#define screen_size 600

///Compilacion g++ `wx-config --cflags` -O2 -o exec main.cpp `wx-config --libs`

int compare (const void * a, const void * b)
{
  if ( *(double*)a <  *(double*)b ) return-1;
  if ( *(double*)a >  *(double*)b ) return 1;
  else return 0;
}

int main(int argc, char **argv)
{
    char *inputfile  = argv[1];
    char *outputfile = argv[2];

    double factor=5.0;
    double offsetx=300;
    double offsety=100;
    int n;
    target T;
    mechanism mecha;

    FILE *input;
    input=fopen(inputfile,"r");
    if (!input){printf("Imposible abrir archivo");return 0;}
    fscanf(input,"%d",&n);
    for (int i=0;i<n;i++)
        fscanf(input,"%lf %lf",&T.x[i],&T.y[i]);
    fclose(input);

    for (int i=0;i<n;i++)
    {
        T.x[i]=factor*T.x[i]+offsetx;
        T.y[i]=screen_size-factor*T.y[i]-offsety;
    }

    FILE *output;
    output=fopen(outputfile,"r");
    if (!output){printf("Imposible abrir archivo");return 0;}

    fscanf(output,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",
          &mecha.r1,&mecha.r2,&mecha.r3,&mecha.r4,&mecha.rcx,&mecha.rcy,&mecha.x0,&mecha.y0,&mecha.theta0);
    mecha.input=(double *)malloc(n*sizeof(double));

    for (int i=0;i<n;i++)
        fscanf(output,"%lf",&mecha.input[i]);
    fscanf(output,"%lf %lf",&mecha.result1,&mecha.result2);

    fclose(output);

    qsort(mecha.input,n,sizeof(double),compare);

    mecha.r1*=factor;
    mecha.r2*=factor;
    mecha.r3*=factor;
    mecha.r4*=factor;
    mecha.rcx*=factor;
    mecha.rcy*=factor;
    mecha.x0=mecha.x0*factor+offsetx;
    mecha.y0=screen_size-mecha.y0*factor-offsety;

    RunSimulation(mecha,T,n);

return 0;
}

