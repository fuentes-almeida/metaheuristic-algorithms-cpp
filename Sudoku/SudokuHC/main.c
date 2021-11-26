#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"

int main(int argc, char**argv)
{
    srand(time(NULL));
    int samples=100;///
    int flag=atoi(argv[1]);
    char *inputfile=argv[2];///
    char *outputfile=argv[3];///

    clock_t start=clock();
    int *Sudoku=(int*)calloc(n*n,sizeof(int));
    Vector **CurrentSol=(Vector**)calloc(n,sizeof(Vector*));
    Vector **CandidateSol=(Vector**)calloc(n,sizeof(Vector*));
    Vector **BestSolution=(Vector**)calloc(n,sizeof(Vector*));

    SetProblemSpace(Sudoku,inputfile,CurrentSol,CandidateSol,BestSolution);

    int *Results=(int*)calloc(samples,sizeof(int));
    for (int i=0;i<samples;i++)
    {
        Results[i]=SudokuHillClimbing(Sudoku,CurrentSol,CandidateSol,BestSolution,flag);
        //printf("cost= %d\n\n",Results[i]);
    }

    double mean;
    FILE *output=fopen(outputfile,"a");
    for (int i=0;i<samples;i++)
    {
        fprintf(output,"%d ",Results[i]);
        mean+=Results[i];
    }
    clock_t end=clock();
    fprintf(output,"%d %s %lf %lf\n",flag,inputfile,mean/samples, (end-start)/(double)CLOCKS_PER_SEC);

    fclose(output);

    free(Sudoku);
    FreeVectorMem(CurrentSol);
    FreeVectorMem(CandidateSol);
    FreeVectorMem(BestSolution);

    return 0;
}
