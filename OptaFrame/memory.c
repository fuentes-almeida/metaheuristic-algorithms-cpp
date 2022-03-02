#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

void *requestSingleArrayMemory(int num_elements, size_t data_size){
    return (void *)calloc(num_elements, data_size);
}

void **requestDoubleArrayMemory(int num_rows, int num_columns, size_t data_size){
    /*
    n=#rows, m=#cols, s=sizeof(int)
    int *x[n][n] -> void *x[n*m*s];
    e.g. n=3, m=3, s=4:

    |[4BYTES]|-------|-------|-------|-------|-------|-------|-------|-------|
    0x00     0x04    0x08    0x0C    0x10    0x14    0x18    0x1C    0x20    0x24
    |------------------------|-----------------------|-----------------------|
    0x00                     0x0C                    0x18

     x[0] = 0*[3]*[4] = 0x00
     x[1] = 1*[3]*[4] = 0x0C
     x[2] = 2*[3]*[4] = 0x18
     x[i] = i*[m]*[s]
    */
    void **ptr = calloc(num_rows, sizeof(void*));
    if (ptr == NULL)
        return NULL;
    ptr[0] = calloc(num_rows*num_columns, data_size);
    if (ptr[0] == NULL)
        return NULL;

    for (int i=1; i<num_rows; i++){
        ptr[i]=ptr[i-1]+num_columns*data_size;
    }
    return ptr;
}

void freeSingleArrayMemory(void *ptr, int num_elements, size_t data_size){
    free(ptr);
    return;
}

void freeDoubleArrayMemory(void **ptr, int num_rows, int num_columns, size_t data_size){
    /*TODO: How to check memory leaks in Win mingW*/
    for (int i=0; i<num_rows; i++){
        freeSingleArrayMemory(ptr[i], num_columns, data_size);
    }
    free(ptr);
    return;
}
