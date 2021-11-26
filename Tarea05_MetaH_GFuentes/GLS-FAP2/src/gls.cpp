#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "memo.h"

long long int OriginalFSFAP_Evaluator(int **Weights, int** Neighbours,int t_num,int* solution)
{
    long long int cost=0;
    for (int i=0;i<t_num;i++)
        for (int j=i+1;j<t_num;j++)
            if(Neighbours[i][j]==1 && abs(solution[i]-solution[j])<=Weights[i][j])
                cost+=Weights[j][i];

    return cost;
}

long long int FSFAP_Evaluator(int **Weights, int** Neighbours,int **Features,int t_num,int* solution, int lambda)
{
    long long int cost=0;
    for (int i=0;i<t_num;i++)
        for (int j=i+1;j<t_num;j++)
            if(Neighbours[i][j]==1 && abs(solution[i]-solution[j])<=Weights[i][j])
            {
                cost+=Weights[j][i];
                ///si la caracteristica esta presente, se penaliza de acuerdo al valor asignado en utilities
                if (Features[j][i]==1)
                    cost+=lambda*Neighbours[j][i];
            }
    return cost;
}

void ComputeUtilities(int *Solution, int **Weights, int** Neighbours, int **Features,int t_num)
{
    int maxUtility=0;

    for (int i=0;i<t_num;i++)
        for (int j=i+1;j<t_num;j++)
            if(Neighbours[i][j]==1 && abs(Solution[i]-Solution[j])<=Weights[i][j] && Features[j][i]==1)
            {
                double aux=Features[i][j]/(double)(1+Neighbours[j][i]);
                if (aux>maxUtility)
                    maxUtility=aux;
            }

    for (int i=0;i<t_num;i++)
        for (int j=i+1;j<t_num;j++)
            if(Neighbours[i][j]==1 && abs(Solution[i]-Solution[j])<=Weights[i][j] && Features[j][i]==1)
            {
                double aux=Features[i][j]/(double)(1+Neighbours[j][i]);
                if (aux==maxUtility)
                    Neighbours[j][i]++;
            }
}

void GenerateGraph(char* filename,int **Weights,int **Neighbours,int **Features, int t_num)
{
    FILE *input=fopen(filename,"r");
    if (!input){printf("File could not be opened");exit(-1);}

    int x,y,threshold=50;
    char not_use;

    ///Lectura del archivo
    while(fscanf(input,"%d",&x)>0){
        fscanf(input,"%d %c %c",&y,&not_use,&not_use);
        fscanf(input,"%d %d",&Weights[x][y],&Weights[y][x]);
        ///Llenado de la matriz de vecindad
        Neighbours[x][y]=1;
    }
    fclose(input);

    ///El numero de vecinos de cada vertice se coloca en la diagonal
    ///de la matriz de vecindad
    for (int k=0;k<t_num;k++){
        for (int i=0;i<k;i++)
            Neighbours[k][k]+=Neighbours[i][k];
        for (int j=k+1;j<t_num;j++)
            Neighbours[k][k]+=Neighbours[k][j];
    }

    ///Inicializacion de caracteristicas
    for (int k=0;k<t_num;k++)
        for (int i=0;i<k;i++)
            if (Weights[k][i]>threshold){
                ///se indica que no se desea la carateristica
                Features[k][i]=1;
                ///coste ci de la caracteristica
                Features[i][k]=Weights[k][i];
            }
}

void RandomSolution(int *Transmitters,int t_num, int freq_num)
{
    for (int i=0;i<t_num;i++)
        Transmitters[i]=rand()%freq_num+1;
}

void CopyVector(int *src, int *dest,int size)
{
    for (int i=0;i<size;i++)
        dest[i]=src[i];
}

void Neighbour(int **Weights, int **Neighbours, int **Features,int *Current, int *Candidate, int t_num, int freq_num,int lambda)
{
    ///se va a ejegir un vertice al azar y se le va a signar la frecuencia que genere la menor penalizacion
    CopyVector(Current,Candidate,t_num);

    int index=rand()%t_num;
    int fnext=Candidate[index];

    while (fnext==Candidate[index])
        fnext=rand()%freq_num+1;

    Candidate[index]=fnext;
}

void FAPHillClimbing(int **Weights, int **Neighbours, int **Features,int *Solution, int t_num,int freq_num, int lambda)
{
    int counter=0;
    long long int cost_curr=0,cost_next=0;

    int *candidate=RequestVectorIntMem(t_num);

    while (counter<10000)
    {
        cost_curr=FSFAP_Evaluator(Weights,Neighbours,Features,t_num,Solution,lambda);

        Neighbour(Weights,Neighbours,Features,Solution,candidate,t_num,freq_num,lambda);

        cost_next=FSFAP_Evaluator(Weights,Neighbours,Features,t_num,candidate,lambda);

        if (cost_next<cost_curr)
        {
            CopyVector(candidate,Solution,t_num);
            cost_curr=cost_next;
            counter=0;
        }

        else counter++;
    }
}

void GuidedLocalSearch(char *filename, int **Weights, int **Neighbours, int **Features, int *Transmitters, int *BestSolution,int t_num, int freq_num, char*outputfile, int lambda)
{
    FILE *output=fopen(outputfile,"w");
    int counter=0,restart=0;
    long long int new_value,old_value,costMin=1e9;

/**************Inico de la Busqueda Local Guiada******************/

    double start,finish,reset;
    start=omp_get_wtime();
    reset=omp_get_wtime();

    ///Paso 1a: se inicializan penaltis en 0 (en Neighbours)
    GenerateGraph(filename,Weights,Neighbours,Features,t_num);

    ///Paso 1b: Solucion inicial
    RandomSolution(Transmitters,t_num,freq_num);

    while(true)
    {
        counter++;
        ///Paso 2: Aplicar metodo de busqueda local
        FAPHillClimbing(Weights,Neighbours,Features,Transmitters,t_num,freq_num,lambda);

        ///se evalua el vector solucion con la funcion original
        new_value=OriginalFSFAP_Evaluator(Weights,Neighbours,t_num,Transmitters);

        ///si es mejor se guarda como mejor solucion
        if (new_value<costMin)
        {
            CopyVector(Transmitters,BestSolution,t_num);
            costMin=new_value;
        }
        if (new_value!=old_value)
        {
            old_value=new_value;
            restart=0;
        }
        else
            restart++;

        if (restart==100)
        {
            RandomSolution(Transmitters,t_num,freq_num);
            //printf("Reset\n");
            restart=0;
        }

        finish=omp_get_wtime();
        //printf("%lf %lld %lld %d %d\n",finish-start,new_value,costMin,counter,lambda);

        if (finish-start>=3600)
            break;
        if (finish-reset>=120)
        {
            fprintf(output,"%lf %lld %d\n",finish-start,costMin,counter);
            reset=omp_get_wtime();
        }

        ///Paso 3: Calcular utilidades y actualizar funcion objetivo
        ComputeUtilities(Transmitters,Weights,Neighbours,Features,t_num);
    }
    fclose(output);
}
