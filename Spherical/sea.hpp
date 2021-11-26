#ifndef SEA_HPP_INCLUDED
#define SEA_HPP_INCLUDED

using namespace std;
typedef double (*function)(int,double*);

void InitPopulation(int N, int d, double **X, double xmin, double xmax);
void EvaluateFitness(int N, int d, double **X, function fx);
void SelecBestIndividuals(int N, int d, int h, double **X, double **C);
void ReplaceIndividuals(int N, int d, double **X, double **Y);
void Inversion(int d, double *c, double *x, double *y);
void Reflection(int d, double *c, double *x, double *y);

double SphericalEA(int N, int d, int h, function fx, double xmin, double xmax);

#endif // SEA_HPP_INCLUDED
