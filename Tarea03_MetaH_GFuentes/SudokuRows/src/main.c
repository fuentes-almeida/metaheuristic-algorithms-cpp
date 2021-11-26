#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"

int main(int argc, char** argv)
{
    srand(time(NULL));
    int flag=atoi(argv[1]);
    int *Datos=(int*)calloc(n*n,sizeof(int));
    Vector **CurrentSol=(Vector**)calloc(n,sizeof(Vector*));
    Vector **CandidateSol=(Vector**)calloc(n,sizeof(Vector*));
    Vector **BestSolution=(Vector**)calloc(n,sizeof(Vector*));

    Sudoku *S=SetProblemSpace(Datos,CurrentSol,CandidateSol,BestSolution);

    for (int i=0;i<100;i++)
    SudokuHillClimbing(S,CurrentSol,CandidateSol,BestSolution,flag);

    FreeVectorMem(CurrentSol);
    FreeVectorMem(CandidateSol);
    FreeVectorMem(BestSolution);

    return 0;
}
