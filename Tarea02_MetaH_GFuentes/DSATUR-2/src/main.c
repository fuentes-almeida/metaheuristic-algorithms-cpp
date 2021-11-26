#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "memo.h"
#include "dsatur.h"

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
    int **Assgnmt_Map=RequestMatrixIntMem(t_num,freq_num);

    GenerateGraph(filename,Weights,Neighbours,t_num);

///Inico de la asignacion de frecuencias

///paso 1: inicializar grados de saturacion a cero
    int *Sat_degree=RequestVectorIntMem(t_num);
    int count=0;

while (count<t_num)
{
    ///paso 2: elegir el vertice con mayor saturacion
    int imax=EdgeSelection(Sat_degree,Neighbours,t_num);

    ///paso 3: buscar la menor frecuencia disponible para asignar
    int fmin=FrequencyAssignment(imax,freq_num,t_num,Assgnmt_Map,Transmitters,Neighbours,Weights);

    ///paso 4: Actualizar frecuencias disponibles,vecindades y grados de saturacion
    UpdateGraph(imax,fmin,t_num,freq_num,Weights,Neighbours,Assgnmt_Map,Sat_degree,Transmitters);

    count++;
}
    printf("Solucion:\n");
    for (int i=0;i<t_num;i++)
        printf("%d ",Transmitters[i]);
    printf("\n");

    ///llamada a funcion del evaluador
    FSFAP_Evaluator(Weights,Neighbours,t_num,Transmitters);

    return 0;
}
