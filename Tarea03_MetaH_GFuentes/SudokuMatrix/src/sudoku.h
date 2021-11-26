#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#define n 9
#define x(a,b) (3*(a/3)+b/3)
#define y(a,b) (3*(a%3)+b%3)

typedef struct Matrix{
    int rows;
    int cols;
    int *data;
}Matrix;

Matrix *RequestMatrixMem(int size1,int size2);
void FreeMatrixMem(Matrix *M);
void SetProblemSpace(Matrix *Sudoku);
int FindValue(Matrix *Sudoku,int k,int row);
void CopySolution(Matrix *Current, Matrix *Next);
void RandomSolution(Matrix *Sudoku,Matrix *Current);
void PrintSudoku(Matrix *Sudoku);
int CalculatePenalty(int *Penalty,Matrix *Current, Matrix *Sudoku,int flag);
void Neighbor(int *Penalty,Matrix *Sudoku,Matrix *Current, Matrix *Next);
void SudokuHillClimbing(Matrix *Sudoku,Matrix *CurrentSol,Matrix *CandidateSol,Matrix *BestSolution,int flag);


#endif // SUDOKU_H_INCLUDED
