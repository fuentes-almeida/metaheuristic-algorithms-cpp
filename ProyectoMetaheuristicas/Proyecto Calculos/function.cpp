#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <bitset>
#include "rand.hpp"
#include "memo.hpp"
#include "function.hpp"
#define SIZE 15

int compare (const void * a, const void * b)
{
  if ( *(double*)a <  *(double*)b ) return-1;
  if ( *(double*)a >  *(double*)b ) return 1;
  else return 0;
}

int compare2 (const void * a, const void * b)
{
    int dim=15;
    double *x=*(double**)a;
    double *y=*(double**)b;

    if (fmin(x[dim],x[dim+1])<fmin(y[dim],y[dim+1])) return-1;
    if (fmin(x[dim],x[dim+1])>fmin(y[dim],y[dim+1])) return 1;
    else return 0;
}

int GetMin(int dim, double *vec)
{
    double mini=1e12;
    int imin;
    for (int i=0;i<dim;i++)
    {
        if (vec[i]<mini)
        {
            mini=vec[i];
            imin=i;
        }
    }
    return imin;
}

int GetMax(int dim, double *vec)
{
    double maxi=0;
    int imax;
    for (int i=0;i<dim;i++)
    {
        if (vec[i]>maxi)
        {
            maxi=vec[i];
            imax=i;
        }
    }
    return imax;
}

int CheckOrder(int start, int dim, double *vec)
{
    for (int i=start; i<start+dim-1; i++)
        if (vec[i%dim]>vec[(i+1)%dim])
            return 0;
    return 1;
}

void CopyIndividual(int dim, double *src, double *dst)
{
    for (int i=0;i<dim;i++)
        dst[i]=src[i];
}

double CalculateDispersion(int N, int dim, double **X)
{
    double *Means=RequestVectorDoubleMem(dim);
    double *StdDevs=RequestVectorDoubleMem(dim);

    for (int i=0;i<N;i++)
        for (int j=0;j<dim;j++)
            Means[j]+=X[i][j];

    for (int i=0;i<dim;i++)
        Means[i]/=N;

    for (int i=0;i<N;i++)
        for (int j=0;j<dim;j++)
            StdDevs[j]+=(X[i][j]-Means[j])*(X[i][j]-Means[j]);

    double mean=0;

    for (int i=0;i<dim;i++)
        mean+=sqrt(StdDevs[i]);
    mean/=dim;

    free(Means);
    free(StdDevs);

    return mean;
}

void RandomPopulation(int N, int dim, double **X)
{
    int num=dim-9;
    double *aux1=RequestVectorDoubleMem(4);
    double *aux2=RequestVectorDoubleMem(num);
    double *aux3=RequestVectorDoubleMem(num);

    for (int i=0;i<N;i++)
    {
        do{
            for (int j=0;j<4;j++)
                aux1[j]=randU(0,MAX);
            qsort(aux1,4,sizeof(double),compare);
           }
        while(aux1[0]+aux1[3]<aux1[1]+aux1[2]);

        for (int j=0;j<4;j++)
            X[i][j]=aux1[j];

        for (int j=4;j<8;j++)
            X[i][j]=randU(0,MAX);

        X[i][8]=randU(0,2*PI);

        for (int j=0;j<num;j++)
            aux2[j]=randU(0,2*PI);

        qsort(aux2,num,sizeof(double),compare);

        int jrand=randI(0,num-1);

        for (int j=0;j<num;j++)
            aux3[(jrand+j)%num]=aux2[j];

        for (int j=9;j<dim;j++)
            X[i][j]=aux3[j-9];
    }
    free(aux1);
    free(aux2);
    free(aux3);
}

void ObjectiveFunction(int dim, double *X, double **TargetPts, double *Fitness)
{
    double theta3_1,theta3_2;

    double k1=X[0]/X[1];
    double k2=X[0]/X[2];
    double k3=(X[3]*X[3]-X[0]*X[0]-X[1]*X[1]-X[2]*X[2])/(2*X[1]*X[2]);

    for (int j=9;j<dim;j++)
    {
        double a=k3+(k2+1)*cos(X[j])-k1;
        double b=-2*sin(X[j]);
        double c=k1+(k2-1)*cos(X[j])+k3;

        if (a!=0 && b*b-4*a*c>=0)
        {
            theta3_1=2*atan2(-b+sqrt(b*b-4*a*c),2*a);
            theta3_2=2*atan2(-b-sqrt(b*b-4*a*c),2*a);

            double Cxr1=X[1]*cos(X[j])+X[4]*cos(theta3_1)-X[5]*sin(theta3_1);
            double Cyr1=X[1]*sin(X[j])+X[4]*sin(theta3_1)+X[5]*cos(theta3_1);

            double Cx1=X[6]+Cxr1*cos(X[8])-Cyr1*sin(X[8]);
            double Cy1=X[7]+Cxr1*sin(X[8])+Cyr1*cos(X[8]);

            double Cxr2=X[1]*cos(X[j])+X[4]*cos(theta3_2)-X[5]*sin(theta3_2);
            double Cyr2=X[1]*sin(X[j])+X[4]*sin(theta3_2)+X[5]*cos(theta3_2);

            double Cx2=X[6]+Cxr2*cos(X[8])-Cyr2*sin(X[8]);
            double Cy2=X[7]+Cxr2*sin(X[8])+Cyr2*cos(X[8]);

            Fitness[0]+=pow(TargetPts[0][j-9]-Cx1,2)+pow(TargetPts[1][j-9]-Cy1,2);
            Fitness[1]+=pow(TargetPts[0][j-9]-Cx2,2)+pow(TargetPts[1][j-9]-Cy2,2);
        }
        else
        {
            Fitness[0]+=1e5;
            Fitness[1]+=1e5;
        }
    }
    if (X[0]+X[1]>=X[2]+X[3])
    {
        Fitness[0]+=1e5;
        Fitness[1]+=1e5;
    }

    int imin=GetMin(dim-9,&X[9]);
    int flag=CheckOrder(imin,dim-9,&X[9]);
    if (flag==0)
    {
        Fitness[0]+=1e5;
        Fitness[1]+=1e5;
    }
}

void EvaluateFitness(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest)
{
    double mini=1e10; int imin;

    for (int i=0;i<N;i++)
        for (int j=0;j<2;j++)
            Fitness[i][j]=0;

    for (int i=0;i<N;i++)
    {
        ObjectiveFunction(dim,X[i],TargetPts,Fitness[i]);

        if (Fitness[i][0]<mini || Fitness[i][1]<mini)
            {
                CopyIndividual(dim,X[i],Elit);
                mini=fmin(Fitness[i][0],Fitness[i][1]);
                imin=i;
            }
    }
    *Fbest=mini;
    *Ibest=imin;
}

void LocalSearch(int dim, double *X, double *Fitness, double **TargetPts, double delta1, double delta2)
{
    double *Neighbour=RequestVectorDoubleMem(dim);
    double *newFitness=RequestVectorDoubleMem(2);
    double *Candidate=RequestVectorDoubleMem(dim);
    double *CandFitness=RequestVectorDoubleMem(2);

    CopyIndividual(dim,X,Candidate);
    CopyIndividual(2,Fitness,CandFitness);

    for (int i=0;i<dim;i++)
    {
        CopyIndividual(dim,X,Neighbour);

        if (i<8) Neighbour[i]+=(2*delta1*rand()/RAND_MAX-delta1);
        else     Neighbour[i]+=(2*delta2*rand()/RAND_MAX-delta2);

        if (i<8 && Neighbour[i]>MAX) Neighbour[i]-=(Neighbour[i]-MAX);
        if (i<8 && Neighbour[i] < 0) Neighbour[i]-=(Neighbour[i]+randU(0,MAX/10));

        ObjectiveFunction(dim,Neighbour,TargetPts,newFitness);
        if (fmin(newFitness[0],newFitness[1])<fmin(CandFitness[0],CandFitness[1]))
            {
                CopyIndividual(dim,Neighbour,Candidate);
                CopyIndividual(2,newFitness,CandFitness);
            }
    }
    CopyIndividual(dim,Candidate,X);
    CopyIndividual(2,CandFitness,Fitness);

    free(Neighbour);
    free(newFitness);
    free(Candidate);
    free(CandFitness);
}

void ImprovePopulation(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest, double delta1, double delta2)
{
    double mini=1e10; int imin;
    for (int i=0;i<N;i++)
    {
        LocalSearch(dim,X[i],Fitness[i],TargetPts,delta1,delta2);
        if (Fitness[i][0]<mini || Fitness[i][1]<mini)
        {
            CopyIndividual(dim,X[i],Elit);
            mini=fmin(Fitness[i][0],Fitness[i][1]);
            imin=i;
        }
    }
    *Fbest=mini;
    *Ibest=imin;
}

void RandomRecombination(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest)
{
    double mini=1e10; int imin;
    int *r=RequestVectorIntMem(3);
    double *NewInd=RequestVectorDoubleMem(dim);
    double *newFitness=RequestVectorDoubleMem(2);
    double *Candidate=RequestVectorDoubleMem(dim);
    double *CandFitness=RequestVectorDoubleMem(2);

    for (int i=0;i<N;i++)
    {
        CopyIndividual(dim,X[i],Candidate);
        CopyIndividual(2,Fitness[i],CandFitness);

        do { r[0]=rand()%N; } while (r[0]==i);
        do { r[1]=rand()%N; } while (r[1]==i || r[1]==r[0]);
        do { r[2]=rand()%N; } while (r[2]==i || r[2]==r[0] || r[2]==r[1]);

        for (int p=0;p<4;p++)
            NewInd[p]=X[r[rand()%3]][p];
        for (int p=4;p<6;p++)
            NewInd[p]=X[r[rand()%3]][p];
        for (int p=6;p<9;p++)
            NewInd[p]=X[r[rand()%3]][p];
        for (int p=9;p<dim;p++)
            NewInd[p]=X[r[rand()%3]][p];

        ObjectiveFunction(dim,NewInd,TargetPts,newFitness);

        double aux=1.0*rand()/RAND_MAX;
        if (i!=*Ibest && aux<0.5)
            {
                CopyIndividual(dim,NewInd,Candidate);
                CopyIndividual(2,newFitness,CandFitness);
            }

        CopyIndividual(dim,Candidate,X[i]);
        CopyIndividual(2,CandFitness,Fitness[i]);

        if (Fitness[i][0]<mini || Fitness[i][1]<mini)
        {
            CopyIndividual(dim,X[i],Elit);
            mini=fmin(Fitness[i][0],Fitness[i][1]);
            imin=i;
        }
    }
    *Fbest=mini;
    *Ibest=imin;

    free(r);
    free(NewInd);
    free(newFitness);
    free(CandFitness);
    free(Candidate);
}

void RecombinationDOR(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest)
{
    double mini=1e10; int imin;
    int *r=RequestVectorIntMem(3);
    double *NewInd=RequestVectorDoubleMem(dim);
    double *newFitness=RequestVectorDoubleMem(2);
    double *Candidate=RequestVectorDoubleMem(dim);
    double *CandFitness=RequestVectorDoubleMem(2);

    for (int i=0;i<N;i++)
    {
        CopyIndividual(dim,X[i],Candidate);
        CopyIndividual(2,Fitness[i],CandFitness);

        do { r[0]=rand()%N; } while (r[0]==i);
        do { r[1]=rand()%N; } while (r[1]==i || r[1]==r[0]);
        do { r[2]=rand()%N; } while (r[2]==i || r[2]==r[0] || r[2]==r[1]);

        for (int j=0;j<3;j++)
            for (int k=0;k<3;k++)
                for (int l=0;l<3;l++)
                    for (int m=0;m<3;m++)
                    {
                        for (int p=0;p<4;p++)
                            NewInd[p]=X[r[j]][p];
                        for (int p=4;p<6;p++)
                            NewInd[p]=X[r[k]][p];
                        for (int p=6;p<9;p++)
                            NewInd[p]=X[r[l]][p];
                        for (int p=9;p<dim;p++)
                            NewInd[p]=X[r[m]][p];

                        ObjectiveFunction(dim,NewInd,TargetPts,newFitness);
                        if (fmin(newFitness[0],newFitness[1])<fmin(CandFitness[0],CandFitness[1]))
                            {
                                CopyIndividual(dim,NewInd,Candidate);
                                CopyIndividual(2,newFitness,CandFitness);
                            }
                    }

        CopyIndividual(dim,Candidate,X[i]);
        CopyIndividual(2,CandFitness,Fitness[i]);

        if (Fitness[i][0]<mini || Fitness[i][1]<mini)
        {
            CopyIndividual(dim,X[i],Elit);
            mini=fmin(Fitness[i][0],Fitness[i][1]);
            imin=i;
        }
    }
    *Fbest=mini;
    *Ibest=imin;

    free(r);
    free(NewInd);
    free(newFitness);
    free(CandFitness);
    free(Candidate);
}

void InitializePopulation(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest, double delta1, double delta2)
{
    RandomPopulation(N,dim,X);
    EvaluateFitness(N,dim,X,Fitness,TargetPts,Elit,Fbest,Ibest);
    ImprovePopulation(N,dim,X,Fitness,TargetPts,Elit,Fbest,Ibest,delta1,delta2);
}

void Mutation(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest, double delta1, double delta2)
{
    for (int i=0;i<N;i++)
    {
        int jrand=rand()%dim;
        if (i!=*Ibest)
        {
            if (jrand<8) X[i][jrand]+=delta1*randN(0,1);
            else         X[i][jrand]+=delta2*randN(0,1);

            if (jrand<8 && X[i][jrand]>MAX) X[i][jrand]-=(X[i][jrand]-MAX);
            if (jrand<8 && X[i][jrand] < 0) X[i][jrand]-= X[i][jrand];
        }

    }
    EvaluateFitness(N,dim,X,Fitness,TargetPts,Elit,Fbest,Ibest);
}

void CompetePopulation(int N, int dim, double **X, double **Xnew, double **Xall, double **F, double **Fnew, double *Elit, double *Fbest, int *Ibest)
{
    for (int i=0;i<N;i++)
    {
        CopyIndividual(dim,X[i],Xall[i]);
        CopyIndividual(dim,Xnew[i],Xall[N+i]);
        Xall[i][dim]=F[i][0];
        Xall[i][dim+1]=F[i][1];
        Xall[N+i][dim]=Fnew[i][0];
        Xall[N+i][dim+1]=Fnew[i][1];
    }

    qsort(Xall,2*N,sizeof(Xall[0]),compare2);

    for (int i=0;i<N;i++)
    {
        CopyIndividual(dim,Xall[i],X[i]);
        CopyIndividual(dim,X[i],Xnew[i]);
        F[i][0]=Fnew[i][0]=Xall[i][dim];
        F[i][1]=Fnew[i][1]=Xall[i][dim+1];
    }

    CopyIndividual(dim,X[0],Elit);
    *Fbest=fmin(F[0][0],F[0][1]);
    *Ibest=0;

}

void RestartPopulation(int N, int dim, double **X, double **Xnew, double **F, double **Fnew, double **TargetPts, double *Elit, double *Fbest, int *Ibest, double delta1, double delta2)
{
    int preserve=10;
    double **Pres=RequestMatrixDoubleMem(preserve,dim);
    double **Fpres=RequestMatrixDoubleMem(preserve,2);

    for (int i=0;i<preserve;i++)
    {
        CopyIndividual(dim,Xnew[i],Pres[i]);
        CopyIndividual(2,Fnew[i],Fpres[i]);
    }

    RandomPopulation(N,dim,X);

    for (int i=0;i<preserve;i++)
        CopyIndividual(dim,Pres[i],X[i]);

    EvaluateFitness(N,dim,X,F,TargetPts,Elit,Fbest,Ibest);

    ImprovePopulation(N,dim,X,F,TargetPts,Elit,Fbest,Ibest,delta1,delta2);

    for (int i=0;i<N;i++)
    {
        CopyIndividual(dim,X[i],Xnew[i]);
        CopyIndividual(2,F[i],Fnew[i]);
    }

    freeMatrizDouble(Fpres);
    freeMatrizDouble(Pres);
}

void MemeticAlgorithm(int N, int num, double **TargetPts, int MaxIte)
{
    int dim=9+num, Ibest, k=0;
    double Fbest, deltamax1=30.0, deltamax2=0.8;
    double **X=RequestMatrixDoubleMem(N,dim);
    double **Xnew=RequestMatrixDoubleMem(N,dim);
    double **Xall=RequestMatrixDoubleMem(2*N,dim+2);
    double **F=RequestMatrixDoubleMem(N,2);
    double **Fnew=RequestMatrixDoubleMem(N,2);
    double *Elit=RequestVectorDoubleMem(dim);

    InitializePopulation(N,dim,X,F,TargetPts,Elit,&Fbest,&Ibest,deltamax1,deltamax2);

    for (int i=0;i<N;i++)
    {
        CopyIndividual(dim,X[i],Xnew[i]);
        CopyIndividual(2,F[i],Fnew[i]);
    }
    FILE *output=fopen("output.dat","a");

    while (k<MaxIte)
    {
        double delta1=deltamax1-(double)k/MaxIte*deltamax1;
        double delta2=deltamax2-(double)k/MaxIte*deltamax2;

        if (k<MaxIte/2)
            RandomRecombination(N,dim,Xnew,Fnew,TargetPts,Elit,&Fbest,&Ibest);
        else
            RecombinationDOR(N,dim,Xnew,Fnew,TargetPts,Elit,&Fbest,&Ibest);

        ImprovePopulation(N,dim,Xnew,Fnew,TargetPts,Elit,&Fbest,&Ibest,delta1,delta2);
        Mutation(N,dim,Xnew,Fnew,TargetPts,Elit,&Fbest,&Ibest,delta1,delta2);

        ImprovePopulation(N,dim,Xnew,Fnew,TargetPts,Elit,&Fbest,&Ibest,delta1,delta2);
        CompetePopulation(N,dim,X,Xnew,Xall,F,Fnew,Elit,&Fbest,&Ibest);

        double mean=CalculateDispersion(N,dim,Xnew);
        if (mean<1.0)
            RestartPopulation(N,dim,X,Xnew,F,Fnew,TargetPts,Elit,&Fbest,&Ibest,delta1,delta2);

        k++;
    }

    for (int i=0;i<dim;i++)
        fprintf(output,"%lf ",Elit[i]);
    fprintf(output,"%lf\n",Fbest);

    fclose(output);
    freeMatrizDouble(X);
    freeMatrizDouble(Xnew);
    freeMatrizDouble(F);
    freeMatrizDouble(Fnew);
    free(Elit);
}
