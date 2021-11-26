#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "memo.h"
#include "dsatur.h"

int GenerateSubGraph(int t_num, int**Weights, int**SubGraph,int **SubNeighbours, int *Sat_degree)
{
    long long int mean=0;
    int num=0,subcount=0;
    for (int i=1;i<t_num;i++)
        for (int j=0;j<i;j++)
            if(Weights[i][j]>0)
            {
                num++;
                mean+=Weights[i][j];
            }
     mean=mean/num;

    for (int i=1;i<t_num;i++)
        for (int j=0;j<i;j++)
            if(Weights[i][j]>mean)
            {
                Sat_degree[i]=1;
                Sat_degree[j]=1;
                SubGraph[i][j]=Weights[i][j];
                SubGraph[j][i]=Weights[j][i];
                SubNeighbours[j][i]=1;
            }
    for (int i=0;i<t_num;i++)
        subcount+=Sat_degree[i];

///vecindad del subgrafo
    for (int k=0;k<t_num;k++){
        for (int i=0;i<k;i++)
            SubNeighbours[k][k]+=SubNeighbours[i][k];
        for (int j=k+1;j<t_num;j++)
            SubNeighbours[k][k]+=SubNeighbours[k][j];
    }

    printf("Sub grafo:\n");
    for (int i=0;i<t_num;i++){
        for (int j=0;j<t_num;j++)
            printf("%d\t",SubGraph[i][j]);
    printf("\n");}
    printf("\n");

    printf("subVecindades:\n");
    for (int i=0;i<t_num;i++){
        for (int j=0;j<t_num;j++)
            printf("%d\t",SubNeighbours[i][j]);
    printf("\n");}
    printf("\n");

    return subcount;
}



int main(int argc, char **argv)
{
    srand(time(NULL)+clock());  ///esta semilla permite mayor aleatoriedad,
                                ///ya que corremos muchas veces seguidas el mismo programa

/** Lectura de parametros
    filename: archivo de entrada GSM2-XXX
    t_num: numero de antenas (vertices) XXX
    freq_num: numero de frecuencias distintas **/

    char* filename=argv[1];
    int t_num=atoi(argv[2]);
    int freq_num=atoi(argv[3]);

    int **Weights=RequestMatrixIntMem(t_num,t_num);
    int *Transmitters=RequestVectorIntMem(t_num);
    int **Neighbours=RequestMatrixIntMem(t_num,t_num);


    GenerateGraph(filename,Weights,Neighbours,t_num);

    DSATURoptimization(Transmitters,Weights,Neighbours,t_num,freq_num);

    printf("Solucion:\n");
    for (int i=0;i<t_num;i++)
        printf("%d ",Transmitters[i]);
    printf("\n");

    int *counter=RequestVectorIntMem(freq_num);
    for (int i=0;i<t_num;i++)
        counter[Transmitters[i]-1]++;
    printf("\n");
    printf("Contador:\n");
    for (int i=0;i<freq_num;i++)
        printf("%d ",counter[i]);
    printf("\n");

    ///llamada a funcion del evaluador
    FSFAP_Evaluator(Weights,Neighbours,t_num,Transmitters);

    return 0;
}
