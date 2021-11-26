#ifndef PSO_HPP_INCLUDED
#define PSO_HPP_INCLUDED
#include <vector>
using namespace std;
typedef double (*function)(int,double*);

int compare (const void * a, const void * b);
void CopyIndividual(int d, double *src, double *dst);
double CalculateDispersion(int N, int dim, double **X);
void CalculateStats(int N, double *vector);

void SelectTopology(int N, int **Top, int flag, int k, int kmax,int *rand);
void UpdateFitness(int N, int d, function fx, double **X, double **Pbest, double **Lbest, double *Elit, int **Top);
void InitPopulation(int N, int d, double **X, double **V, double **Pbest, double **Lbest, function fx, double xmin, double xmax);
void UpdateParticles(int N, int d, double **X, double **V, double **Pbest, double **Lbest, double xmin, double xmax, double c1, double c2);

double PSO(int N, int d, function fx, double xmin, double xmax, double c1, double c2, int flag);

#endif // PSO_HPP_INCLUDED
