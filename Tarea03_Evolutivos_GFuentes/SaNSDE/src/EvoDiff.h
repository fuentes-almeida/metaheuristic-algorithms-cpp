#ifndef EVODIFF_H_INCLUDED
#define EVODIFF_H_INCLUDED
#include <vector>
using namespace std;
typedef double (*function)(int,double*);

double DifferentialEvol(int N, int d, function fx, double xmin, double xmax);

void InitPopulation(int N, int d, double xmin, double xmax, double **P, function fx, double *Fitnesss, int *Ibest, double *Fbest);

void Mutation(int N, int d, double xmin, double xmax, double **X, double **V, int Ibest, double Prob, double fp, int *MutEquation, int *Distr);
void Crossover(int N, int d, double **X, double **V, double **U, int jrand, int *MutEquation, int* Ecounter, int *Distr, int *Fcounter, double *CRis);
void Selection(int N, int d, double **X, double **U, function fx, double *Fitness, int *Ibest, double *Fbest,int *MutEquation, int *Ecounter, int *Distr, int *Fcounter, double *CRis, vector<double> *CRrec, vector<double> *deltaF);

double updateProb(int *counter);
void CopyIndividual(int d, double *src, double *dst);
double updateCR(vector <double> *CRrec, vector <double> *deltaF);

#endif // EVODIFF_H_INCLUDED
