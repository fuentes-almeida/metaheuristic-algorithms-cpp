#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#define test_num 288
#define freq_num 2612
#define resets 50
#define file_num 30
#define baseLog log(19)
#define freq_values 19
#define start_freq 134


int **RequesMatrixIntMem(int rows, int cols);
double **RequesMatrixDoubleMem(int rows, int cols);
double *RequestVectorDoubleMem(int size);
int compare (const void * a, const void * b);
int Hamming(int *array1, int *array2, int size);

int ReadData(char *filename, int **Data, double **Results, int num);

void CalculateMean(double **Results, double *Mean);
void CalculateMedian(double **Results, double *Median);
void CalculateStdDeviation(double **Results,double *Mean,double *StdDev);
void CalculateMaxMin(double **Results,double *Max, double *Min);
void CalculateEntropy(int **Data, double *Probs, double **Entropy,int num);
void CalculateFullEntropy(double **Entropy, double *FullEntropy);
void CalculateClosestDistance(int **Data, double **Hamming,int num);
void CalculateMeanHammingDistance(double **Hamming, double *FullHamming);
void CalculateBestResults(double *BestResults, double **Results);
void CalculateRLD(double **Results,double q_ref,double *RLD);

#endif // TEST_H_INCLUDED
