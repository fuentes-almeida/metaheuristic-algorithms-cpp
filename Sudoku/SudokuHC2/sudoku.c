#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"


Matrix *RequestMatrixMem(int size1,int size2)
{
    Matrix *M=(Matrix*)malloc(sizeof(Matrix));
    M->rows=size1;
    M->cols=size2;
    M->data=(int*)calloc(size1*size2,sizeof(int));
    return M;
}

void FreeMatrixMem(Matrix *M)
{
    free(M->data);
    free(M);
}

void SetProblemSpace(char *inputfile,Matrix *Sudoku)
{
    FILE *input=fopen(inputfile,"r");
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            fscanf(input,"%d",&Sudoku->data[i*n+j]);
    fclose(input);
}

int FindValue(Matrix *Sudoku,int k,int row)
{
    for (int i=0;i<n;i++)
    {
        if (Sudoku->data[row*n+i]==k)
            return 1;
    }
    return 0;
}

void CopySolution(Matrix *Current, Matrix *Next)
{
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            Next->data[i*n+j]=Current->data[i*n+j];
}

void RandomSolution(Matrix *Sudoku,Matrix *Current)
{
    CopySolution(Sudoku,Current);

    int *integers=(int*)calloc(n,sizeof(int));
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            if (Sudoku->data[i*n+j]>0)
                integers[Sudoku->data[i*n+j]-1]++;

    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
        {
            int flag=0;
            if (Current->data[i*n+j]==0)
                while (flag==0)
                {
                    int aux=rand()%9+1;
                    if (integers[aux-1]<n)
                    {
                        Current->data[i*n+j]=aux;
                        flag=1;
                    }

                }
        }
}

void PrintSudoku(Matrix *Sudoku)
{
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++)
            printf("%d ",Sudoku->data[i*n+j]);
        printf("\n");}
    printf("\n");
}

int CalculatePenalty(int *Penalty,Matrix *Current, Matrix *Sudoku,int flag)
{
    int cost=0;
    for(int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            Penalty[i*n+j]=0;

    for(int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            for (int k=j+1;k<n;k++)
            {
                if (Current->data[j*n+i]==Current->data[k*n+i])
                {
                    if (flag>0 && (Sudoku->data[j*n+i]>0 || Sudoku->data[k*n+i]>0))
                    {
                        Penalty[j*n+i]+=4;
                        Penalty[k*n+i]+=4;
                        cost+=8;
                    }
                    Penalty[j*n+i]++;
                    Penalty[k*n+i]++;
                    cost+=2;
                }
                if (Current->data[n*x(i,j)+y(i,j)]==Current->data[n*x(i,k)+y(i,k)])
                {
                    if (flag>0 && (Sudoku->data[n*x(i,j)+y(i,j)]>0 || Sudoku->data[n*x(i,k)+y(i,k)]>0))
                    {
                        Penalty[n*x(i,j)+y(i,j)]+=4;
                        Penalty[n*x(i,k)+y(i,k)]+=4;
                        cost+=8;
                    }
                    Penalty[n*x(i,j)+y(i,j)]++;
                    Penalty[n*x(i,k)+y(i,k)]++;
                    cost+=2;
                }
            }
    return cost;
}

void Neighbor(int *Penalty,Matrix *Sudoku,Matrix *Current, Matrix *Next)
{
    int imax,jmax,pmax=0;
    int countmax=0;
    for (int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if (Sudoku->data[i*n+j]==0)
            {
                if (Penalty[i*n+j]>pmax)
                {
                    pmax=Penalty[i*n+j];
                    countmax=0;
                }
                if (Penalty[i*n+j]==pmax)
                    countmax++;
            }

    int aux=rand()%countmax+1;

    countmax=0;
    for (int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if (Sudoku->data[i*n+j]==0)
            {
                if (Penalty[i*n+j]==pmax && countmax<aux)
                    countmax++;
                if (countmax==aux)
                {
                    imax=i;
                    jmax=j;
                    countmax++;
                }
            }

    CopySolution(Current,Next);

    int swapx,swapy;
    do {
    swapx=rand()%n;
    swapy=rand()%n;
    }while(Sudoku->data[swapx*n+swapy]>0 || ((swapx==imax) & (swapy==jmax))||Next->data[swapx*n+swapy]==Next->data[imax*n+jmax]);


    aux=Next->data[imax*n+jmax];
    Next->data[imax*n+jmax]=Next->data[swapx*n+swapy];
    Next->data[swapx*n+swapy]=aux;

}

void PrintMatrix (int *Matrix)
{
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++)
            printf("%d ",Matrix[i*n+j]);
        printf("\n");}
    printf("\n");
}

int SudokuHillClimbing(Matrix *Sudoku,Matrix *CurrentSol,Matrix *CandidateSol,Matrix *BestSolution,int flag)
{
    int restart=0,cost_curr=0,cost_next=0,cost_best=1000,count=0;
    int *Penalty=(int*)calloc(n*n,sizeof(int));
    RandomSolution(Sudoku,CurrentSol);
    //PrintSudoku(CurrentSol);

    while (restart<1000)
    {
        if (cost_curr==0)
        cost_curr=CalculatePenalty(Penalty,CurrentSol,Sudoku,flag);
        //PrintMatrix(Penalty);
        Neighbor(Penalty,Sudoku,CurrentSol,CandidateSol);
        //PrintSudoku(CandidateSol);
        cost_next=CalculatePenalty(Penalty,CandidateSol,Sudoku,flag);

        if (cost_next<cost_curr){
            CopySolution(CandidateSol,CurrentSol);
            cost_curr=cost_next;
            count=0;
        }
        else count++;

        if (count==30){
            if (cost_curr<cost_best){
                CopySolution(CurrentSol,BestSolution);
                cost_best=cost_curr;
            }
            RandomSolution(Sudoku,CurrentSol);
            restart++;
            cost_curr=0;
            count=0;
        }
    }

    //PrintSudoku(BestSolution);
    //printf("\n");
    return cost_best;

    free(Penalty);
}
