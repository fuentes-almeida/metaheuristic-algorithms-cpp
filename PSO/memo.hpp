#ifndef MEMO_HPP_INCLUDED
#define MEMO_HPP_INCLUDED

int *RequestVectorIntMem(int data_size);
char *RequestVectorCharMem(int data_size);
double *RequestVectorDoubleMem(int data_size);

int **RequestMatrixIntMem(int data_size1, int data_size2);
double **RequestMatrixDoubleMem(int data_size1, int data_size2);

void freeMatrizInt(int **pointer);
void freeMatrizDouble(double **pointer);

#endif // MEMO_HPP_INCLUDED
