#ifndef PSO_HPP_INCLUDED
#define PSO_HPP_INCLUDED
#include <vector>
using namespace std;
typedef double (*function)(int,double*);

double PSO(int N, int d, function fx, double xmin, double xmax, double c1, double c2);
void InitPopulation(int N, int d, double **X, double **V, double **P, function fx, double xmin, double xamx, double *Best);
void UpdateParticles(int N, int d, double **X, double **V, double **P, double *Best, double xmin, double xmax, double c1, double c2);
void UpdateFitness(int N, int d, function fx, double **X, double **P, double *Best);


#endif // PSO_HPP_INCLUDED
