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

long long int FSFAP_Evaluator(int **Weights, int** Neighbours,int **Features,int t_num,int* solution, int *costmax, int *countmax, int lambda)
{
    int aux1=0, aux2=0;
    long long int cost=0;
    for (int i=0;i<t_num;i++)
        for (int j=i+1;j<t_num;j++)
            if(Neighbours[i][j]==1 && abs(solution[i]-solution[j])<=Weights[i][j])
            {
                cost+=Weights[j][i];

                ///esto es para saber cual es la penalizacion maxima y cuantas veces aparece
                if (Weights[j][i]>aux1)
                {
                    aux2=0;
                    aux1=Weights[j][i];
                }
                if (Weights[j][i]==aux1)
                    aux2++;

                ///si la caracteristica esta presente, se penaliza de acuerdo al valor asignado en utilities
                if (Features[j][i]==1)
                    cost+=lambda*Neighbours[j][i];
            }
    *costmax=aux1;
    *countmax=aux2;
    return cost;
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

    /*printf("Grafo penalti-peso:\n");
    for (int i=0;i<t_num;i++){
        for (int j=0;j<t_num;j++)
            printf("%d\t",Weights[i][j]);
    printf("\n");}
    printf("\n");*/

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

    /*printf("Vecindades:\n");
    for (int i=0;i<t_num;i++){
        for (int j=0;j<t_num;j++)
            printf("%d\t",Neighbours[i][j]);
    printf("\n");}
    printf("\n");

    printf("Features:\n");
    for (int i=0;i<t_num;i++){
        for (int j=0;j<t_num;j++)
            printf("%d\t",Features[i][j]);
    printf("\n");}
    printf("\n");*/
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


void Neighbour(int **Weights, int **Neighbours, int *Current, int *Candidate, int t_num, int freq_num,int *costmax, int *countmax)
{
    ///se copia la solucion original
    CopyVector(Current,Candidate,t_num);

    ///se extrae el valor de la mayor penalizacion y cuantas veces aparece
    int aux1=*countmax,aux2=*costmax;
    ///se elige aleatoriamente la penalizacion que se va a resolver
    aux1=rand()%aux1+1;

    int counter=0,imax,jmax,flag=0;

    for (int i=0;i<t_num;i++)
    {
        if (flag==1) break;
        for (int j=i+1;j<t_num;j++)
            if(Neighbours[i][j]==1 && abs(Candidate[i]-Candidate[j])<=Weights[i][j])
            {
                if ((Weights[j][i]==aux2) && (counter<aux1))
                    counter++;

                if (counter==aux1)
                    {
                        ///se guardan los indices que se van a resolver
                        imax=i;
                        jmax=j;
                        flag=1;
                        break;
                    }
            }
    }

    ///se asignan frequencias aleatorias a modo de resolver el conflicto
    while (abs(Candidate[imax]-Candidate[jmax])<=Weights[imax][jmax])
    {
        Candidate[imax]=rand()%freq_num+1;
        Candidate[jmax]=rand()%freq_num+1;
    }
}

void Utilities(int *Solution, int **Weights, int** Neighbours, int **Features,int t_num)
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

void FAPHillClimbing(int **Weights, int **Neighbours, int **Features,int *Solution, int t_num,int freq_num, int lambda)
{
    int counter=0,costmax, countmax;
    long long int cost_curr=0,cost_next=0;

    int *candidate=RequestVectorIntMem(t_num);

    while (counter<1000)
    {
        cost_curr=FSFAP_Evaluator(Weights,Neighbours,Features,t_num,Solution,&costmax,&countmax,lambda);

        Neighbour(Weights,Neighbours,Solution,candidate,t_num,freq_num,&costmax,&countmax);

        cost_next=FSFAP_Evaluator(Weights,Neighbours,Features,t_num,candidate,&costmax,&countmax,lambda);

        if (cost_next<cost_curr)
        {
            CopyVector(candidate,Solution,t_num);
            cost_curr=cost_next;
            counter=0;
        }

        else counter++;
    }
}

void GuidedLocalSearch(char *filename, int **Weights, int **Neighbours, int **Features, int *Transmitters, int *BestSolution,int t_num, int freq_num, char*outputfile)
{
    FILE *output=fopen(outputfile,"w");
    int lambda=1e5,costMin=1e9;
    int counter=0;

/**************Inico de la Busqueda Local Guiada******************/

    double start,finish,reset;
    //int notuse1,notuse2;
    start=omp_get_wtime();
    reset=omp_get_wtime();

    ///Paso 1a: se inicializan penaltis en 0 (en Neighbours)
    GenerateGraph(filename,Weights,Neighbours,Features,t_num);

    ///Paso 1b: Solucion inicial
    //DSATURoptimization(Transmitters,Weights,Neighbours,t_num,freq_num);
    RandomSolution(Transmitters,t_num,freq_num);

    while(true)
    {
        counter++;
        ///Paso 2: Aplicar metodo de busqueda local
        FAPHillClimbing(Weights,Neighbours,Features,Transmitters,t_num,freq_num,lambda);

        ///se evalua el vector solucion con la funcion original
        int aux=OriginalFSFAP_Evaluator(Weights,Neighbours,t_num,Transmitters);

        ///si es mejor se guarda como mejor solucion
        if (aux<costMin)
        {
            CopyVector(Transmitters,BestSolution,t_num);
            costMin=aux;
        }

        finish=omp_get_wtime();

        if (finish-start>=3600) break;
        if (finish-reset>=120)
        {
            /*printf("Solution at %lf sec:\n",finish-start);
            for (int i=0;i<t_num;i++)
                printf("%d ",BestSolution[i]);
            printf("\n");*/
            fprintf(output,"%lf %d %d\n",finish-start,costMin,counter);
            printf("%lf %d %d\n",finish-start,costMin,counter);
            reset=omp_get_wtime();
        }

        /*aux=FSFAP_Evaluator(Weights,Neighbours,Features,t_num,Transmitters,&notuse1,&notuse2,lambda);
        printf("%d antes de utilities\n",aux);*/

        ///Paso 3: Calcular utilidades y actualizar funcion objetivo
        Utilities(Transmitters,Weights,Neighbours,Features,t_num);

        /*aux=FSFAP_Evaluator(Weights,Neighbours,Features,t_num,Transmitters,&notuse1,&notuse2,lambda);
        printf("%d despues de utilities\n",aux);*/
    }
    fclose(output);
}
