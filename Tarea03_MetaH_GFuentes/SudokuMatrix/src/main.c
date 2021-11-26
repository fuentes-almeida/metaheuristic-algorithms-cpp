#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"

int main(int argc, char** argv)
{
    srand(time(NULL));
    int flag=atoi(argv[1]);
    Matrix *Sudoku=RequestMatrixMem(n,n);
    Matrix *CurrentSol=RequestMatrixMem(n,n);
    Matrix *CandidateSol=RequestMatrixMem(n,n);
    Matrix *BestSolution=RequestMatrixMem(n,n);

    SetProblemSpace(Sudoku);
    for (int i=0;i<100;i++)
    SudokuHillClimbing(Sudoku,CurrentSol,CandidateSol,BestSolution,flag);

    FreeMatrixMem(Sudoku);
    FreeMatrixMem(CurrentSol);
    FreeMatrixMem(CandidateSol);
    FreeMatrixMem(BestSolution);

    return 0;
}
