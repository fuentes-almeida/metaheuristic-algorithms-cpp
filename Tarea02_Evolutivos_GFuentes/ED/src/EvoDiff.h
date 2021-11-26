#ifndef EVODIFF_H_INCLUDED
#define EVODIFF_H_INCLUDED

typedef double (*function)(int,double*);

double DifferentialEvol(int N, int d, function fx, double xmin, double xmax, double CR, double Factor);

void InitPopulation(int N, int d, double xmin, double xmax, double **P, function fx, double *Fitnesss, int *Ibest, double *Fbest);

void CopyIndividual(int d, double *src, double *dst);
void Mutation(int N, int d, double xmin, double xmax, double **X, double **V, int Ibest, double Factor);
void Crossover(int N, int d, double **X, double **V, double **U, double CR, int jrand);
void Selection(int N, int d, double **X, double **U, function fx, double *Fitness, int *Ibest, double *Fbest);

#endif // EVODIFF_H_INCLUDED
