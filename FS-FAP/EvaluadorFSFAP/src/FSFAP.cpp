#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FSFAP.hpp"

/** filename: archivo de entrada GSM2-XXX
    ant_num: numero de antenas XXX
    freq_num: numero de frecuencias distintas
    output: archivo de salida
    max_ite: numero de iteraciones a evaluar **/

void FSFAP_Generator(int ant_num,int freq_num, int* vec_Ant)
{
    for (int i=0;i<ant_num;i++)
    vec_Ant[i]=rand()%freq_num+1;
}

void FSFAP_Evaluator(char* filename,int ant_num, int freq_num,char* result,int max_ite)
{
    int *best=(int*)malloc(ant_num*sizeof(int));
    int* vec_Ant=(int*)malloc(ant_num*sizeof(int));

    long long int min_cost=1e15;
    int aux1,aux2,weight,penalty;
    char not_use;

    for (int k=0;k<max_ite;k++)
    {
        long long int cost=0;

        ///Se genera un solucion diferente en cada iteracion
        FSFAP_Generator(ant_num,freq_num,vec_Ant);

        ///Lectura del archivo de instancia
        FILE *input=fopen(filename,"r");
        while(fscanf(input,"%d",&aux1)>0)
        {
            fscanf(input,"%d %c %c %d %d",&aux2,&not_use,&not_use,&weight,&penalty);
            ///verificacion de penalizaciones
            if (abs(vec_Ant[aux1]-vec_Ant[aux2])<=weight)
                cost+=penalty;
        }
        fclose(input);

        ///Vamos conservando la mejor solucion
        if (cost<min_cost)
        {
            min_cost=cost;
            for (int i=0;i<ant_num;i++)
                best[i]=vec_Ant[i];
        }
    }

    ///Escritura del archivo de salida
    FILE *output=fopen(result,"a");
    fprintf(output,"%d %d %lld\n",ant_num,freq_num,min_cost);
    /*for (int i=0;i<ant_num;i++)
        fprintf(output,"%d ",best[i]);
    fprintf(output,"\n\n");*/
    
    fclose(output);
    free(vec_Ant);
    free(best);
}
