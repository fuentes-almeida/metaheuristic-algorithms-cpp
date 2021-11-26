#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"

Vector *RequestVectorMem(int data_size){
    Vector *V=(Vector*)malloc(sizeof(Vector));
    V->size=data_size;
    V->data=(int*)calloc(data_size,sizeof(int));
    return V;
}

void FreeVectorMem(Vector **V){
    for (int i=0;i<n;i++)
    {
        free(V[i]->data);
        free(V[i]);
    }
    free(V);
}

void SetProblemSpace(int *Sudoku,char *inputfile,Vector **CurrentSol, Vector **CandidateSol,Vector **BestSolution)
{
    FILE *input=fopen(inputfile,"r");

    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            fscanf(input,"%d",&Sudoku[i*n+j]);
    fclose(input);
    //PrintMatrix(Sudoku);

    for (int i=0;i<n;i++)
    {
        int count=0;
        for (int j=0;j<n;j++)
            if (Sudoku[i*n+j]==0)
                count++;
        CurrentSol[i]=RequestVectorMem(count);
        CandidateSol[i]=RequestVectorMem(count);
        BestSolution[i]=RequestVectorMem(count);
    }
}


void PermuteVector(Vector *V)
{
    for (int i=0;i<2*V->size;i++)
    {
        int aux1=rand()%V->size;
        int aux2=rand()%V->size;
        int aux3=V->data[aux1];
        V->data[aux1]=V->data[aux2];
        V->data[aux2]=aux3;
    }
}


void PrintSudoku(int *Sudoku,Vector **Solution)
{
    int *index=(int*)calloc(n,sizeof(int));
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            int aux=Sudoku[i*n+j];
            if (aux==0){
                printf ("%d ",Solution[i]->data[index[i]]);
                index[i]++;
            }
            else
                printf("%d ",aux);
        }
        printf("\n");
    }
    printf("\n");
}

int FindValue(int *Sudoku,int row, int value)
{
    for (int i=0;i<n;i++)
    {
        if (Sudoku[row*n+i]==value)
            return 1;
    }
    return 0;
}

void RandomSolution(int *Sudoku,Vector **Solution)
{
    for (int i=0;i<n;i++)
    {
        int index=0;
        for (int k=1;k<=n;k++)
            if (FindValue(Sudoku,i,k))
                continue;
            else{
                Solution[i]->data[index]=k;
                index++;
            }
        PermuteVector(Solution[i]);
    }
}

int CalculatePenalty(int *Penalty,Vector **Current, int *Sudoku,int flag)
{
    int cost=0;
    int *Datos=(int*)calloc(n*n,sizeof(int));
    int *index=(int*)calloc(n,sizeof(int));

    for(int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            Penalty[i*n+j]=0;

    for(int i=0;i<n;i++)
        for (int j=0;j<n;j++)
        {
            int value=Sudoku[i*n+j];
            if (value==0)
            {
                value=Current[i]->data[index[i]];
                index[i]++;
            }
            Datos[i*n+j]=value;
        }
    free(index);

    for(int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            for (int k=j+1;k<n;k++)
            {
                if (Datos[j*n+i]==Datos[k*n+i])
                {
                    if (flag>0 && (Sudoku[j*n+i]>0 || Sudoku[k*n+i]>0))
                    {
                        Penalty[j*n+i]+=4;
                        Penalty[k*n+i]+=4;
                        cost+=8;
                    }
                    Penalty[j*n+i]++;
                    Penalty[k*n+i]++;
                    cost+=2;
                }
                if (Datos[n*x(i,j)+y(i,j)]==Datos[n*x(i,k)+y(i,k)])
                {
                    if (flag>0 && (Sudoku[n*x(i,j)+y(i,j)]>0 || Sudoku[n*x(i,k)+y(i,k)]>0))
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

void CopySolution(Vector **Current, Vector **Next)
{
    for (int i=0;i<n;i++)
        for (int j=0;j<Current[i]->size;j++)
            Next[i]->data[j]=Current[i]->data[j];
}

void Neighbour(int *Penalty,Vector **Current, Vector **Next, int *Sudoku)
{
    int imax,jmax,pmax=0;
    int countmax=0;
    for (int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if (Sudoku[i*n+j]==0)
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
            if (Sudoku[i*n+j]==0)
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

    int swap;
    do{
        swap=rand()%n;
    }
    while (Sudoku[n*imax+swap]>0 || swap==jmax);

    int index=0,index1,index2;
    for (int j=0;j<n;j++)
    {
        if (Sudoku[n*imax+j]==0)
            index++;
        if (j==jmax)
             index1=index-1;
        if (j==swap)
             index2=index-1;
    }

    int aux2=Next[imax]->data[index1];
    Next[imax]->data[index1]=Next[imax]->data[index2];
    Next[imax]->data[index2]=aux2;

}

void PrintMatrix (int *Matrix)
{
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++)
            printf("%d ",Matrix[i*n+j]);
        printf("\n");}
    printf("\n");
}


int SudokuHillClimbing(int *Sudoku,Vector **CurrentSol, Vector **CandidateSol,Vector **BestSolution,int flag)
{
    int restart=0,cost_curr=0,cost_next=0,cost_best=1000,count=0;
    int *Penalty=(int*)calloc(n*n,sizeof(int));
    RandomSolution(Sudoku,CurrentSol);
    //PrintSudoku(Sudoku,CurrentSol);

    while (restart<1000)
    {
        if (cost_curr==0)
        cost_curr=CalculatePenalty(Penalty,CurrentSol,Sudoku,flag);
        //PrintMatrix(Penalty);
        Neighbour(Penalty,CurrentSol,CandidateSol,Sudoku);
        //PrintSudoku(Sudoku,CandidateSol);
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

    //PrintSudoku(Sudoku,BestSolution);
    //printf("\n");
    return cost_best;

    free(Penalty);
}
