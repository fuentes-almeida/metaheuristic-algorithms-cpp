#ifndef MEMO_HPP_INCLUDED
#define MEMO_HPP_INCLUDED


int *RequestVectorIntMem(int);
char *RequestVectorCharMem(int);
double *RequestVectorDoubleMem(int);

int **RequestMatrixIntMem(int,int);
double **RequestMatrixDoubleMem(int,int);

void freeMatrizInt(int**);
void freeMatrizDouble(double**);



#endif // MEMO_HPP_INCLUDED

