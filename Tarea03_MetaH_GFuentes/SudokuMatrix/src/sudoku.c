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

void SetProblemSpace(Matrix *Sudoku)
{
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            scanf("%d",&Sudoku->data[i*n+j]);
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

    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
        {
            int aux=0;
            if (Current->data[i*n+j]==0)
            {
                while (FindValue(Current,aux,i)==1)
                    aux=rand()%9+1;
                Current->data[i*n+j]=aux;
            }
        }
}

void PrintSudoku(Matrix *Sudoku)
{
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++)
            printf("%d ",Sudoku->data[i*n+j]);
        printf("\n");}
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
                        Penalty[j*n+i]+=5;
                        Penalty[k*n+i]+=5;
                        cost+=10;
                    }
                    Penalty[j*n+i]++;
                    Penalty[k*n+i]++;
                    cost+=2;
                }
                if (Current->data[n*x(i,j)+y(i,j)]==Current->data[n*x(i,k)+y(i,k)])
                {
                    if (flag>0 && (Sudoku->data[n*x(i,j)+y(i,j)]>0 || Sudoku->data[n*x(i,k)+y(i,k)]>0))
                    {
                        Penalty[n*x(i,j)+y(i,j)]+=5;
                        Penalty[n*x(i,k)+y(i,k)]+=5;
                        cost+=10;
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
    for (int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if (Sudoku->data[i*n+j]==0)
            {
                if (Penalty[i*n+j]>pmax)
                {
                    pmax=Penalty[i*n+j];
                    imax=i;
                    jmax=j;
                }
                if (Penalty[i*n+j]==pmax && rand()%2==1)
                {
                    imax=i;
                    jmax=j;
                }
            }

        }

    CopySolution(Current,Next);

    int swap;
    do{
        swap=rand()%n;
    }
    while (Sudoku->data[imax*n+swap]>0 || swap==jmax);

    int aux=Next->data[imax*n+jmax];
    Next->data[imax*n+jmax]=Next->data[imax*n+swap];
    Next->data[imax*n+swap]=aux;

}

void SudokuHillClimbing(Matrix *Sudoku,Matrix *CurrentSol,Matrix *CandidateSol,Matrix *BestSolution,int flag)
{
    int restart=0,cost_curr=0,cost_next=0,cost_best=1000,count=0;
    int *Penalty=(int*)calloc(n*n,sizeof(int));
    RandomSolution(Sudoku,CurrentSol);

    while (restart<1000)
    {
        if (cost_curr==0)
        cost_curr=CalculatePenalty(Penalty,CurrentSol,Sudoku,flag);
        Neighbor(Penalty,Sudoku,CurrentSol,CandidateSol);
        cost_next=CalculatePenalty(Penalty,CandidateSol,Sudoku,flag);

        if (cost_next<cost_curr){
            CopySolution(CandidateSol,CurrentSol);
            cost_curr=cost_next;
            count=0;
        }
        else count++;

        if (count==20){
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
    printf("%d\n",cost_best);

    free(Penalty);
}
