#include <cstdlib>
#include "memo.hpp"

int *RequestVectorIntMem(int data_size)
{
    int *pointer;
    pointer=(int*)calloc(data_size,sizeof(int));
    return pointer;
}

char *RequestVectorCharMem(int data_size)
{
    char *pointer;
    pointer=(char*)calloc(data_size,sizeof(char));
    return pointer;
}

double *RequestVectorDoubleMem(int data_size)
{
    double *pointer;
    pointer=(double*)calloc(data_size,sizeof(double));
    return pointer;
}

int **RequestMatrixIntMem(int data_size1, int data_size2)
{
    int **pointer;
    pointer=(int **)calloc(data_size1,sizeof(int*));
    if (pointer==NULL) return NULL;

    pointer[0]=(int*)calloc(data_size1*data_size2,sizeof(int));
    if (pointer[0]==NULL) return NULL;

    for (int i=1;i<data_size1;i++)
        pointer[i]=pointer[i-1]+data_size2;

    return pointer;
}

double **RequestMatrixDoubleMem(int data_size1, int data_size2)
{
    double **pointer;
    pointer=(double **)calloc(data_size1,sizeof(double*));
    if (pointer==NULL) return NULL;

    pointer[0]=(double*)calloc(data_size1*data_size2,sizeof(double));
    if (pointer[0]==NULL) return NULL;

    for (int i=1;i<data_size1;i++)
        pointer[i]=pointer[i-1]+data_size2;

    return pointer;
}

void freeMatrizInt(int **pointer){
    free(pointer[0]);
    free(pointer);
}

void freeMatrizDouble(double **pointer){
    free(pointer[0]);
    free(pointer);
}


