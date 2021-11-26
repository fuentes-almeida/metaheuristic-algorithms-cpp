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

Sudoku *SetMemory(int data_size){
    Sudoku *S=(Sudoku*)malloc(sizeof(Sudoku));
    S->numbers=RequestVectorMem(data_size);
    S->X=RequestVectorMem(data_size);
    S->Y=RequestVectorMem(data_size);
    return S;
}

void FreeSudoku(Sudoku *S){
    free(S->numbers);
    free(S->X);
    free(S->Y);
    free(S);
}

Sudoku *SetProblemSpace(int *Datos,Vector **CurrentSol, Vector **CandidateSol,Vector **BestSolution)
{
    int count=0,aux,index=0;
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
        {
            scanf("%d",&aux);
            if (aux!=0){
                count++;
                Datos[i*n+j]=aux;
                }
        }
    Sudoku *S=SetMemory(count);

    for (int i=0;i<n;i++)
    {
        count=0;
        for (int j=0;j<n;j++)
            if (Datos[i*n+j]==0)
                count++;
            else{
                S->numbers->data[index]=Datos[i*n+j];
                S->X->data[index]=i;
                S->Y->data[index]=j;
                index++;
            }
        CurrentSol[i]=RequestVectorMem(count);
        CandidateSol[i]=RequestVectorMem(count);
        BestSolution[i]=RequestVectorMem(count);
    }
    free(Datos);
return S;

}

int FindValue(int value, Sudoku *S,int row){

    for (int i=0;i<S->numbers->size;i++)
        if (S->X->data[i]==row && S->numbers->data[i]==value)
            return 1;
    return 0;
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

int ReturnValue(Sudoku *S,int k, int p)
{
    for (int i=0;i<S->numbers->size;i++)
        if (S->X->data[i]==k && S->Y->data[i]==p)
            return S->numbers->data[i];
    return 0;
}

void PrintSudoku(Sudoku *S,Vector **Solution)
{
    int *index=(int*)calloc(n,sizeof(int));
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            int aux=ReturnValue(S,i,j);
            if (aux==0){
                printf ("%d ",Solution[i]->data[index[i]]);
                index[i]++;
            }
            else
                printf("%d ",aux);
        }
        printf("\n");
    }
}

void RandomSolution(Sudoku *S,Vector **Solution)
{
    for (int i=0;i<n;i++)
    {
        int index=0;
        for (int k=1;k<=n;k++)
            if (FindValue(k,S,i))
                continue;
            else{
                Solution[i]->data[index]=k;
                index++;
            }
        PermuteVector(Solution[i]);
    }
}

int CalculatePenalty(int *Penalty,Vector **Current, Sudoku *S,int flag)
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
            int value=ReturnValue(S,i,j);
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
                    if (flag>0 && (ReturnValue(S,j,i)>0 || ReturnValue(S,k,i)>0))
                    {
                        Penalty[j*n+i]+=5;
                        Penalty[k*n+i]+=5;
                        cost+=10;
                    }
                    Penalty[j*n+i]++;
                    Penalty[k*n+i]++;
                    cost+=2;
                }
                if (Datos[n*x(i,j)+y(i,j)]==Datos[n*x(i,k)+y(i,k)])
                {
                    if (flag>0 && (ReturnValue(S,x(i,j),y(i,j))>0 || ReturnValue(S,x(i,k),y(i,k))>0))
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

void CopySolution(Vector **Current, Vector **Next)
{
    for (int i=0;i<n;i++)
        for (int j=0;j<Current[i]->size;j++)
            Next[i]->data[j]=Current[i]->data[j];
}

void Neighbor(int *Penalty,Vector **Current, Vector **Next, Sudoku *S)
{
    int imax,jmax,pmax=0;
    for (int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if (ReturnValue(S,i,j)==0)
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
    while (ReturnValue(S,imax,swap)>0 || swap==jmax);

    int index=0,index1,index2;
    for (int j=0;j<n;j++)
    {
        if (ReturnValue(S,imax,j)==0)
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

void SudokuHillClimbing(Sudoku *S,Vector **CurrentSol, Vector **CandidateSol,Vector **BestSolution,int flag)
{
    int restart=0,cost_curr=0,cost_next=0,cost_best=1000,count=0;
    int *Penalty=(int*)calloc(n*n,sizeof(int));
    RandomSolution(S,CurrentSol);

    while (restart<1000)
    {
        if (cost_curr==0)
        cost_curr=CalculatePenalty(Penalty,CurrentSol,S,flag);
        Neighbor(Penalty,CurrentSol,CandidateSol,S);
        cost_next=CalculatePenalty(Penalty,CandidateSol,S,flag);

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
            RandomSolution(S,CurrentSol);
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
