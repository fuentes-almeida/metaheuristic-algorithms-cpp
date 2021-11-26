#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "memo.h"
#include "dsatur.h"

void FSFAP_Evaluator(int **Weights, int** Neighbours,int t_num,int* solution)
{
    long long int cost=0;
    for (int i=0;i<t_num;i++)
        for (int j=i+1;j<t_num;j++)
            if(Neighbours[i][j]==1 && abs(solution[i]-solution[j])<=Weights[i][j])
            cost+=Weights[j][i];

    printf("\n\nCosto minimo:\n");
    printf("%lld\n",cost);
}

void GenerateGraph(char* filename,int **Weights,int **Neighbours,int t_num)
{
    FILE *input=fopen(filename,"r");
    if (!input){printf("File could not be opened");exit(-1);}

    int x,y;
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

}

int EdgeSelection(int *Sat_degree, int** Neighbours,int t_num)
{
    int max=-1,imax=-1; ///imax=indice con mayor saturacion
    for (int i=0;i<t_num;i++)
        {
        if (Sat_degree[i]>max){
            max=Sat_degree[i];
            imax=i;
            }
        if (imax<0) continue;
        ///si hay mas de un vertice con maxima saturacion
        ///elegimos el vertice que tenga mayor vecindad
        if (Sat_degree[i]==max && Neighbours[i][i]>=Neighbours[imax][imax])
            imax=i;
        }
    return imax;
}

int FrequencyAssignment(int imax,int freq_num,int t_num,int **Assgnmt_Map,int *Transmitters, int**Neighbours, int**Weights)
{
    int fmin,not_available=1;
    for (int freq=0;freq<freq_num;freq++)
        if (Assgnmt_Map[imax][freq]==0)
        {
            fmin=freq;
            Assgnmt_Map[imax][fmin]=1;
            Transmitters[imax]=fmin+1;
            not_available=0;
            break;
        }

    ///si no hay una frecuencia disponible,
    ///elegimos la que genere menor penalizacion
    if (not_available==1)
    {
        long long int min=1e9;
        for (int freq=1;freq<=freq_num;freq++){
            long long int cost=0;
            for (int i=0;i<imax;i++)
                if (Neighbours[i][imax]==1 && Transmitters[i]>0)
                    {
                        if (abs(Transmitters[i]-freq)<=Weights[i][imax])
                            cost+=Weights[imax][i];
                    }
            for (int i=imax+1;i<t_num;i++)
                if (Neighbours[imax][i]==1 && Transmitters[i]>0)
                    {
                        if (abs(Transmitters[i]-freq)<=Weights[imax][i])
                            cost+=Weights[i][imax];
                    }
            if (cost<min){
                fmin=freq-1;
                min=cost;
            }
        }
        Assgnmt_Map[imax][fmin]=1;
        Transmitters[imax]=fmin+1;
    }
    return fmin;
}

void UpdateGraph(int imax,int fmin, int t_num, int freq_num,int**Weights, int** Neighbours, int **Assgnmt_Map, int* Sat_degree, int* Transmitters)
{
    for (int i=0;i<imax;i++)
        for (int j=fmin-Weights[i][imax];j<=fmin+Weights[i][imax];j++)
            if (j>=0 && Neighbours[i][imax]==1)
                Assgnmt_Map[i][j]=1;

    for (int i=imax+1;i<t_num;i++)
        for (int j=fmin-Weights[imax][i];j<=fmin+Weights[imax][i];j++)
            if (j>=0 && Neighbours[imax][i]==1)
                Assgnmt_Map[i][j]=1;

    Neighbours[imax][imax]=0;
    for (int i=0;i<imax;i++)
        if (Neighbours[i][imax]==1 && Neighbours[i][i]!=0)
            Neighbours[i][i]--;

    for (int i=imax+1;i<t_num;i++)
        if (Neighbours[imax][i]==1 && Neighbours[i][i]!=0)
            Neighbours[i][i]--;

    ///paso 5: Actualizar saturacion

    Sat_degree[imax]=-1;

    for (int i=0;i<t_num;i++)
        if (Sat_degree[i]>=0)
        {
            Sat_degree[i]=0;
            for (int j=0;j<freq_num;j++)
                Sat_degree[i]+=Assgnmt_Map[i][j];
        }
}
