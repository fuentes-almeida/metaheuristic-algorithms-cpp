#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#define n 9
#define x(a,b) (3*(a/3)+b/3)
#define y(a,b) (3*(a%3)+b%3)

typedef struct Vector{
    int size;
    int *data;
}Vector;

typedef struct Sudoku{
    Vector *numbers;
    Vector *X;
    Vector *Y;
}Sudoku;

Vector *RequestVectorMem(int data_size);
void FreeVectorMem(Vector **V);
Sudoku *SetMemory(int data_size);
void FreeSudoku(Sudoku *S);
Sudoku *SetProblemSpace(int *Datos,Vector **CurrentSol, Vector **CandidateSol,Vector **BestSolution);
int FindValue(int value, Sudoku *S,int row);
void PermuteVector(Vector *V);
int ReturnValue(Sudoku *S,int k, int p);
void PrintSudoku(Sudoku *S,Vector **Solution);
void RandomSolution(Sudoku *S,Vector **Solution);
int CalculatePenalty(int *Penalty,Vector **Current, Sudoku *S,int flag);
void CopySolution(Vector **Current, Vector **Next);
void Neighbor(int *Penalty,Vector **Current, Vector **Next, Sudoku *S);
void SudokuHillClimbing(Sudoku *S,Vector **CurrentSol, Vector **CandidateSol,Vector **BestSolution,int flag);

#endif // SUDOKU_H_INCLUDED
