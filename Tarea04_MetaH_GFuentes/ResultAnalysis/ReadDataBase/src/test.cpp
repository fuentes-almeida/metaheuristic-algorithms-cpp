#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include "test.h"
using namespace std;

int **RequesMatrixIntMem(int rows, int cols)
{
    int **matrix=new int*[rows];
    matrix[0]=new int[rows*cols];

    for (int i=1;i<rows;i++)
        matrix[i]=matrix[i-1]+cols;

    for (int i=0;i<rows;i++)
        for (int j=0;j<cols;j++)
            matrix[i][j]=0;

    return matrix;
}

double **RequesMatrixDoubleMem(int rows, int cols)
{
    double **matrix=new double*[rows];
    matrix[0]=new double[rows*cols];

    for (int i=1;i<rows;i++)
        matrix[i]=matrix[i-1]+cols;

    for (int i=0;i<rows;i++)
        for (int j=0;j<cols;j++)
            matrix[i][j]=0;

    return matrix;
}

double *RequestVectorDoubleMem(int size)
{
    double *vector=new double[size];
    for (int i=0;i<size;i++)
        vector[i]=0;
    return vector;
}

int compare (const void * a, const void * b)
{
       if ( *(double*)a <  *(double*)b ) return -1;
  else if ( *(double*)a == *(double*)b ) return 0;
  else return 1;

}

int HammingDistance(int *array1, int *array2, int size)
{
    int distance=0;

    for (int i=0;i<size;i++)
        if (array1[i]!=array2[i])
            distance++;

    return distance;
}

int ReadData(char *filename, int **Data, double **Results, int num)
{
    ifstream input(filename);
    if (!input.is_open()) {cout<<"Unable to open file"<<endl;return 0;}

    string not_use;
    char aux;
    for (int i=0;i<test_num;i++)
    {
        getline(input,not_use);
        getline(input,not_use);
        for (int j=0;j<resets;j++)
        {
            for (int k=0;k<freq_num;k++)
                input>>Data[resets*i+j][k];
            input>>Results[num*resets+j][i];
        }
        input.get(aux);
        input.get(aux);
    }
    input.close();

    /*for (int i=0;i<test_num*resets;i++){
        for (int j=0;j<freq_num;j++)
            cout<<Data[i][j]<<" ";
        cout<<endl;}
    cout<<endl;
    for (int i=0;i<resets*file_num;i++){
        for (int j=0;j<test_num;j++)
            cout<<Results[i][j]<<" ";
        cout<<endl;}
    cout<<endl;*/

    return 1;
}

void CalculateMean(double **Results, double *Mean)
{
    for (int i=0;i<test_num;i++)
    {
        for (int j=0;j<file_num;j++)
            Mean[i]+=Results[j*resets][i];
        Mean[i]/=file_num;
    }

    /*for (int i=0;i<test_num;i++)
        cout<<Mean[i]<<" ";
    cout<<endl;*/
}

void CalculateMedian(double **Results, double *Median)
{
    double *aux=RequestVectorDoubleMem(file_num);

    for (int i=0;i<test_num;i++)
    {
        for (int j=0;j<file_num;j++)
            aux[j]=Results[j*resets][i];

        qsort(aux,file_num,sizeof(double),compare);
        if (file_num%2==0)
        Median[i]=(aux[file_num/2-1]+aux[file_num/2])/2;
        else Median[i]=aux[file_num/2];
    }
    delete [] aux;

    /*for (int i=0;i<test_num;i++)
        cout<<Median[i]<<" ";
    cout<<endl;*/
}

void CalculateStdDeviation(double **Results,double *Mean,double *StdDev)
{
    for (int i=0;i<test_num;i++)
    {
        for (int j=0;j<file_num;j++)
            StdDev[i]+=pow(Results[j*resets][i]-Mean[i],2);
        StdDev[i]=sqrt(StdDev[i]/(file_num-1));
    }

    /*for (int i=0;i<test_num;i++)
        cout<<StdDev[i]<<" ";
    cout<<endl;*/
}

void CalculateMaxMin(double **Results,double *Max, double *Min)
{
    double maxi,mini;
    for (int i=0;i<test_num;i++)
    {
        for (int j=0;j<file_num;j++)
        {
            maxi=0;
            mini=1e10;
            for (int k=0;k<resets;k++)
            {
                if (Results[j*resets+k][i]>maxi)
                    maxi=Results[j*resets+k][i];
                if (Results[j*resets+k][i]<mini)
                    mini=Results[j*resets+k][i];
            }
            Max[i]+=maxi;
            Min[i]+=mini;
        }
        Max[i]/=file_num;
        Min[i]/=file_num;
    }

    /*for (int i=0;i<test_num;i++)
        cout<<Max[i]<<" ";
    cout<<endl;

    for (int i=0;i<test_num;i++)
        cout<<Min[i]<<" ";
    cout<<endl;*/
}

void CalculateEntropy(int **Data, double *Probs, double **Entropy,int num)
{
    for (int k=0;k<test_num;k++)
        for (int i=0;i<freq_num;i++)
        {
            for (int i=0;i<freq_values;i++)
                Probs[i]=0;

            for (int j=0;j<resets;j++)
              Probs[Data[k*resets+j][i]-start_freq]+=1.0;

            for (int p=0;p<freq_values;p++)
                    Probs[p]/=resets;

            for (int p=0;p<freq_values;p++)
            {
                if (Probs[p]>0)
                Entropy[num][k]-=log(Probs[p])*Probs[p]/baseLog;
            }
        }
    for (int i=0;i<test_num;i++)
        Entropy[num][i]/=freq_num;

    /*for (int k=0;k<file_num;k++){
        for (int i=0;i<test_num;i++)
        cout<<Entropy[k][i]<<" ";
    cout<<endl;}
    cout<<endl;*/
}

void CalculateFullEntropy(double **Entropy, double *FullEntropy)
{
    for (int i=0;i<test_num;i++)
    {
        for (int j=0;j<file_num;j++)
            FullEntropy[i]+=Entropy[j][i];
        FullEntropy[i]/=file_num;
    }

    /*for (int i=0;i<test_num;i++)
        cout<<FullEntropy[i]<<" ";
    cout<<endl;*/
}

void CalculateClosestDistance(int **Data, double **Hamming,int num)
{
    int **dist_matrix=RequesMatrixIntMem(resets,resets);

    for (int i=0;i<test_num;i++)
    {
        for (int j=0;j<resets;j++)
        {
            for (int k=j+1;k<resets;k++)
            {
                dist_matrix[j][k]=HammingDistance(Data[i*resets+j],Data[i*resets+k],freq_num);
                dist_matrix[k][j]=dist_matrix[j][k];
            }
        }

    /*for (int k=0;k<resets;k++){
        for (int i=0;i<resets;i++)
        cout<<dist_matrix[k][i]<<" ";
    cout<<endl;}
    cout<<endl;*/

        double dist_mean=0;
        for (int j=0;j<resets;j++)
        {
            int minim=1e6;
            for (int k=0;k<resets;k++)
            {
                if (k!=j && dist_matrix[j][k]<minim)
                    minim=dist_matrix[j][k];
            }
            dist_mean+=minim;
        }
        Hamming[num][i]=dist_mean/resets;

    }

    /*for (int k=0;k<file_num;k++){
        for (int i=0;i<test_num;i++)
        cout<<Hamming[k][i]<<" ";
    cout<<endl;}
    cout<<endl;*/
}

void CalculateMeanHammingDistance(double **Hamming, double *FullHamming)
{
    for (int i=0;i<test_num;i++)
    {
        for (int j=0;j<file_num;j++)
            FullHamming[i]+=Hamming[j][i];
        FullHamming[i]/=file_num;
    }

    /*for (int i=0;i<test_num-1;i++)
        cout<<FullHamming[i]<<" ";
    cout<<endl;*/
}

void CalculateBestResults(double *BestResults, double **Results)
{
    for (int i=0;i<file_num;i++)
        BestResults[i]=Results[i*resets][test_num-1];

    /*for (int i=0;i<file_num;i++)
        cout<<BestResults[i]<<" ";
    cout<<endl;*/
}

void CalculateRLD(double **Results,double q_ref,double *RLD)
{
    for (int i=0;i<test_num;i++)
    {
        double counter=0;
        for (int j=0;j<file_num;j++)
        {
            if (Results[resets*j][i]<q_ref)
                counter=counter+1.0;
        }
        RLD[i]=counter/file_num;
    }
}
