#ifndef MEMORY_H
#define MEMORY_H

typedef unsigned char BYTE;

/*These functions are used to request dynamic memory for one and two-dimensional arrays
  a void pointer will be returned which should be casted to the data type being used*/
void  *requestSingleArrayMemory(int, size_t);
void **requestDoubleArrayMemory(int, int, size_t);

/*These other functions are used to free dynamic memory for same data structures*/
void freeSingleArrayMemory(void  *, int, size_t);
void freeDoubleArrayMemory(void **, int, int, size_t);

#endif // MEMO_H

