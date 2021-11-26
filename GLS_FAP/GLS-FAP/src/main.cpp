#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "memo.h"
#include "gls.h"

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
    char *outputfile=argv[4];

    int **Weights=RequestMatrixIntMem(t_num,t_num);
    int *Transmitters=RequestVectorIntMem(t_num);
    int **Neighbours=RequestMatrixIntMem(t_num,t_num);
    int **Features=RequestMatrixIntMem(t_num,t_num);
    int *BestSolution=RequestVectorIntMem(t_num);

    GuidedLocalSearch(filename,Weights,Neighbours,Features,Transmitters,BestSolution,t_num,freq_num,outputfile);

    freeMatrizInt(Weights,t_num);
    freeMatrizInt(Neighbours,t_num);
    freeMatrizInt(Features,t_num);
    free(Transmitters);
    free(BestSolution);

    return 0;
}
