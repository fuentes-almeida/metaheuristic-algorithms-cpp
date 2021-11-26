#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include "test.h"

using namespace std;

int main(int argc, char**argv)
{
    string filename=argv[1];
    string outputfile=argv[2];
    string ResultsFile=argv[3];
    string Identifier=argv[4];

    int **Data=RequesMatrixIntMem(test_num*resets,freq_num);
    double **Results=RequesMatrixDoubleMem(resets*file_num,test_num);
    double *Mean=RequestVectorDoubleMem(test_num);
    double *Median=RequestVectorDoubleMem(test_num);
    double *StdDev=RequestVectorDoubleMem(test_num);
    double *Max=RequestVectorDoubleMem(test_num);
    double *Min=RequestVectorDoubleMem(test_num);
    double **Entropy=RequesMatrixDoubleMem(file_num,test_num);
    double *Probs=RequestVectorDoubleMem(freq_values);
    double *FullEntropy=RequestVectorDoubleMem(test_num);
    double **Hamming=RequesMatrixDoubleMem(file_num,test_num);
    double *FullHamming=RequestVectorDoubleMem(test_num);
    double *RLD=RequestVectorDoubleMem(test_num);
    double *BestResults=RequestVectorDoubleMem(file_num);

    for (int i=0;i<file_num;i++)
    {
        string inputfile= filename + to_string(i);
        int flag=ReadData(&inputfile[0],Data,Results,i);
        if (flag==0) return 0;

        CalculateEntropy(Data,Probs,Entropy,i);
        CalculateClosestDistance(Data,Hamming,i);
    }

    CalculateBestResults(BestResults,Results);
    CalculateMean(Results,Mean);
    CalculateRLD(Results,Mean[60],RLD);
    CalculateMedian(Results,Median);
    CalculateStdDeviation(Results,Mean,StdDev);
    CalculateMaxMin(Results,Max,Min);
    CalculateFullEntropy(Entropy,FullEntropy);
    CalculateMeanHammingDistance(Hamming,FullHamming);

    ofstream output(outputfile);
    output<<"Mean\tMedian\tStdDev\tMax\tMin\tEntropy\tHamming\tRLD\n";
    output.precision(10);
    for (int i=0;i<test_num;i++)
    {
        output<<fixed<<Mean[i]<<"\t"<<Median[i]<<"\t"<<StdDev[i]<<"\t"<<Max[i]<<"\t"
              <<Min[i]<<"\t"<<FullEntropy[i]<<"\t"<<FullHamming[i]<<"\t"<<RLD[i]<<"\t"<<endl;
    }
    output.close();

    ofstream Result(ResultsFile,ios::out|ios::app);
    Result<<Identifier;
    Result.precision(10);
    for (int i=0;i<file_num;i++)
        Result<<"\t"<<fixed<<BestResults[i];
    Result<<endl;
    Result.close();

    delete [] Data[0];
    delete [] Data;
    delete [] Results[0];
    delete [] Results;
    delete [] Entropy[0];
    delete [] Entropy;
    delete [] Hamming[0];
    delete [] Hamming;
    delete [] Mean;
    delete [] Median;
    delete [] StdDev;
    delete [] Max;
    delete [] Min;
    delete [] FullEntropy;
    delete [] Probs;
    delete [] FullHamming;
    delete [] BestResults;
    delete [] RLD;

    return 0;
}
