#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FSFAP.hpp"

int main(int argc, char **argv)
{
    srand(time(NULL)+clock());  ///esta semilla permite mayor aleatoriedad,
                                ///ya que corremos muchas veces seguidas el mismo programa

/** Lectura de parametros
    filename: archivo de entrada GSM2-XXX
    ant_num: numero de antenas XXX
    freq_num: numero de frecuencias distintas **/

    char* filename=argv[1];
    int ant_num=atoi(argv[2]);
    int freq_num=atoi(argv[3]);
    char* output=argv[4];

    ///llamada a funcion del evaluador
    FSFAP_Evaluator(filename,ant_num,freq_num,output,1000);

    return 0;
}
