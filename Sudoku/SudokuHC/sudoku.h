#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#define n 9
#define x(a,b) (3*(a/3)+b/3)
#define y(a,b) (3*(a%3)+b%3)

typedef struct Vector{
    int size;
    int *data;
}Vector;

Vector *RequestVectorMem(int data_size);
void FreeVectorMem(Vector **V);
void PrintMatrix (int *Matrix);
void SetProblemSpace(int *Datos,char* inputfile,Vector **CurrentSol, Vector **CandidateSol, Vector **BestSolution);
void PermuteVector(Vector *V);
void RandomSolution(int *Sudoku,Vector **Solution);
int ReturnValue(int *Sudoku,int k, int p);
void PrintSudoku(int *Sudoku,Vector **Solution);
int CalculatePenalty(int *Penalty,Vector **Solution, int *Sudoku,int flag);
void CopySolution(Vector **Current, Vector **Next);
void Neighbour(int *Penalty,Vector **Current, Vector **Next, int *Sudoku);
int SudokuHillClimbing(int *Sudoku,Vector **CurrentSol, Vector **CandidateSol,Vector **BestSolution,int flag);


#endif // SUDOKU_H_INCLUDED
