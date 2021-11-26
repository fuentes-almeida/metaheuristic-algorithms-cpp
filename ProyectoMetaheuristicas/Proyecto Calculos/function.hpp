#ifndef FUNCTION_HPP_INCLUDED
#define FUNCTION_HPP_INCLUDED

#define MAX 60
#define MIN -60
#define PI 3.141592

using namespace std;

int compare (const void * a, const void * b);
int compare2 (const void * a, const void * b);
int GetMin(int dim, double *vec);
int GetMax(int dim, double *vec);
int CheckOrder(int start, int dim, double *vec);
void CopyIndividual(int dim, double *src, double *dst);
double CalculateDispersion(int N, int dim, double **X);
void RandomPopulation(int N, int dim, double **X);
void ObjectiveFunction(int dim, double *X, double **TargetPts, double *Fitness);
void EvaluateFitness(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest);
void LocalSearch(int dim, double *X, double *Fitness, double **TargetPts, double delta1, double delta2);
void ImprovePopulation(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest, double delta1, double delta2);
void RandomRecombination(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest);
void RecombinationDOR(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest);
void InitializePopulation(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest, double delta1, double delta2);
void Mutation(int N, int dim, double **X, double **Fitness, double **TargetPts, double *Elit, double *Fbest, int *Ibest, double delta1, double delta2);
void CompetePopulation(int N, int dim, double **X, double **Xnew, double **Xall, double **F, double **Fnew, double *Elit, double *Fbest, int *Ibest);
void RestartPopulation(int N, int dim, double **X, double **Xnew, double **F, double **Fnew, double **TargetPts, double *Elit, double *Fbest, int *Ibest, double delta1, double delta2);

void MemeticAlgorithm(int N, int num, double **TargetPts, int MaxIte);

#endif // FUNCTION_HPP_INCLUDED
